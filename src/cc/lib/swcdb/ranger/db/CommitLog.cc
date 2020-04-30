/*
 * Copyright Since 2019 SWC-DB© [author: Kashirin Alex kashirin.alex@gmail.com]
 */


#include "swcdb/ranger/db/CommitLog.h"
#include "swcdb/core/Encoder.h"
#include "swcdb/core/Time.h"
#include "swcdb/core/Semaphore.h"


namespace SWC { namespace Ranger { namespace CommitLog {



Fragments::Fragments(const Types::KeySeq key_seq)  
                    : m_cells(key_seq), stopping(false), 
                      m_commiting(false), m_deleting(false), 
                      m_compacting(false) { 
}

void Fragments::init(RangePtr for_range) {
  SWC_ASSERT(for_range != nullptr);
  
  range = for_range;
  
  m_cells.configure(
    range->cfg->block_cells()*2,
    range->cfg->cell_versions(), 
    range->cfg->cell_ttl(), 
    range->cfg->column_type()
  );
}

Fragments::~Fragments() { }

void Fragments::schema_update() {
  std::scoped_lock lock(m_mutex_cells);
  m_cells.configure(
    range->cfg->block_cells()*2,
    range->cfg->cell_versions(), 
    range->cfg->cell_ttl(), 
    range->cfg->column_type()
  );
}

void Fragments::add(const DB::Cells::Cell& cell) {
  bool roll;
  {
    std::scoped_lock lock(m_mutex_cells);
    m_cells.add_raw(cell);
    roll = _need_roll();
  }

  if(roll && m_mutex.try_lock()) {
    if(!m_deleting && !m_commiting) {
      m_commiting = true;
      asio::post(*Env::IoCtx::io()->ptr(), [this](){ commit_new_fragment(); });
    }
    m_mutex.unlock();
  }
}

void Fragments::commit_new_fragment(bool finalize) {
  if(finalize) {
    std::unique_lock lock_wait(m_mutex);
    if(m_commiting || m_compacting)
      m_cv.wait(lock_wait, [this] {
        return !m_compacting && !m_commiting && (m_commiting = true); });
  }
  
  Fragment::Ptr frag; 
  Semaphore sem(5);
  for(int err = Error::OK; ;err = Error::OK) {

    DynamicBuffer cells;
    frag = Fragment::make(
      get_log_fragment(Time::now_ns()), 
      range->cfg->key_seq, 
      Fragment::State::WRITING
    );
    
    {
      std::scoped_lock lock(m_mutex);
      for(;;) {
        {
          std::scoped_lock lock2(m_mutex_cells);
          m_cells.write_and_free(
            cells, 
            frag->cells_count, frag->interval, 
            range->cfg->block_size(), range->cfg->block_cells());
        }
        if(cells.fill() >= range->cfg->block_size() || 
           frag->cells_count >= range->cfg->block_cells())
          break;
        {
          std::shared_lock lock2(m_mutex_cells);
          if(!finalize && m_cells.empty())
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        {
          std::shared_lock lock2(m_mutex_cells);
          if(m_cells.empty())
            break;
        }
      }
      if(m_deleting || !cells.fill()) {
        delete frag;
        break;
      }
      m_fragments.push_back(frag);
    }
    
    frag->write(
      err, 
      range->cfg->file_replication(), 
      range->cfg->block_enc(), 
      cells, 
      range->cfg->cell_versions(),
      &sem
    );

    sem.wait_until_under(5);

    std::shared_lock lock2(m_mutex_cells);
    if(!m_cells.size() || (!finalize && !_need_roll()))
      break;
  }

  sem.wait_all();
  {
    std::unique_lock lock_wait(m_mutex);
    m_commiting = false;
  }

  if(!finalize)
    try_compact(false);
  m_cv.notify_all();
}

bool Fragments::try_compact(bool before_major, int tnum) {
  if(!range->compact_possible())
    return false;

  std::unique_lock lock(m_mutex);
  if(stopping)
    return false;

  uint32_t max_compact = range->cfg->log_rollout_ratio();
  if(max_compact < MIN_COMPACT)
    max_compact = MIN_COMPACT;

  std::vector<std::vector<Fragment::Ptr>> groups;
  size_t sz = _need_compact(max_compact, groups);
  bool threshold = sz > (m_fragments.size()/100)*range->cfg->compact_percent();

  if(before_major && !threshold && need_compact_major()) {
    range->compacting(Range::COMPACT_NONE);
    return false;
  }

  if(sz > max_compact * 2 || (sz > max_compact && threshold)) {
    m_compacting = true;
    auto state = threshold? Range::COMPACT_PREPARING  // mitigate add
                          : Range::COMPACT_COMPACTING;// continue scan & add
    if(threshold) {
      range->compact_require(true);
      RangerEnv::compaction_schedule(10000);
    }
    range->compacting(state); 
    new Compact(
      this, m_cells.key_seq, tnum, 
      groups, state, max_compact, m_fragments.size());
    return true;
  }

  range->compacting(Range::COMPACT_NONE);
  need_compact_major();
  return false;
}

void Fragments::finish_compact(const Compact* compact) {
  range->compacting(Range::COMPACT_NONE); 
  m_compacting = false;
  m_cv.notify_all();

  if(!stopping && 
     (compact->repetition >= compact->nfrags / compact->max_compact ||
      !try_compact(false, compact->repetition+1)) ) {
    RangerEnv::compaction_schedule(10000);
  }
  delete compact;
}

bool Fragments::need_compact_major() {
  size_t sz_bytes = 0;
  size_t ok = range->cfg->cellstore_size()/100;
  ok *= range->cfg->compact_percent();
  for(auto frag : m_fragments) {
    if((sz_bytes += frag->size_bytes_encoded()) > ok) {
      range->compact_require(true);
      RangerEnv::compaction_schedule(10000);
      return true;
    }
  }
  return false;
}

const std::string Fragments::get_log_fragment(const int64_t frag) const {
  std::string s(range->get_path(Range::LOG_DIR));
  s.append("/");
  s.append(std::to_string(frag));
  s.append(".frag");
  return s;
}

const std::string Fragments::get_log_fragment(const std::string& frag) const {
  std::string s(range->get_path(Range::LOG_DIR));
  s.append("/");
  s.append(frag);
  return s;
}

void Fragments::load(int &err) {
  std::scoped_lock lock(m_mutex);
  // fragments header OR log.data >> file.frag(intervals)

  err = Error::OK;
  FS::DirentList fragments;
  Env::FsInterface::interface()->readdir(
    err, range->get_path(Range::LOG_DIR), fragments);
  if(err)
    return;

  std::sort(
    fragments.begin(), fragments.end(),
    [](const FS::Dirent& f1, const FS::Dirent& f2) {
      return f1.name.compare(f2.name) < 0; }
  );

  Fragment::Ptr frag;
  for(auto entry : fragments) {
    frag = Fragment::make(get_log_fragment(entry.name), range->cfg->key_seq);
    frag->load_header(true);
    if((err = frag->error()) == Error::FS_PATH_NOT_FOUND) {
      delete frag;
      err = Error::OK;
      continue;
    }
    m_fragments.push_back(frag);
    if(err)
      return;
  }
}

void Fragments::expand(DB::Cells::Interval& intval) {
  std::shared_lock lock(m_mutex);
  for(auto frag : m_fragments)
    intval.expand(frag->interval);
}

void Fragments::expand_and_align(DB::Cells::Interval& intval) {
  std::shared_lock lock(m_mutex);
  for(auto frag : m_fragments) {
    intval.expand(frag->interval);
    intval.align(frag->interval);
  }
}

void Fragments::load_cells(BlockLoader* loader, bool final, int64_t after_ts,
                           std::vector<Fragment::Ptr>& fragments) {  
  if(final) {
    std::unique_lock lock_wait(m_mutex);
    if(m_commiting)
      m_cv.wait(lock_wait, [this]{ return !m_commiting; });
  }

  std::shared_lock lock(m_mutex);
  for(auto frag : m_fragments) {
    if(after_ts < frag->ts && loader->block->is_consist(frag->interval)) {
      fragments.push_back(frag);
      if(fragments.size() == BlockLoader::MAX_FRAGMENTS)
        return;
    }
  }
}

void Fragments::load_cells(BlockLoader* loader) {
  std::shared_lock lock(m_mutex_cells);
  loader->block->load_cells(m_cells);
}

void Fragments::get(std::vector<Fragment::Ptr>& fragments) {
  fragments.clear();
  
  std::shared_lock lock(m_mutex);
  fragments.assign(m_fragments.begin(), m_fragments.end());
}

size_t Fragments::release(size_t bytes) {   
  size_t released = 0;
  std::shared_lock lock(m_mutex);

  for(auto frag : m_fragments) {
    released += frag->release();
    if(bytes && released >= bytes)
      break;
  }
  return released;
}

void Fragments::remove(int &err, std::vector<Fragment::Ptr>& fragments_old) {
  std::scoped_lock lock(m_mutex);

  for(auto old = fragments_old.begin(); old < fragments_old.end(); ++old){
    for(auto it = m_fragments.begin(); it < m_fragments.end(); ++it) {
      if(*it == *old) {
        (*it)->remove(err);
        delete *it;
        m_fragments.erase(it);
        break;
      }
    }
  }
}

void Fragments::remove(int &err, Fragment::Ptr frag, bool remove_file) {
  std::scoped_lock lock(m_mutex);
  for(auto it = m_fragments.begin(); it < m_fragments.end(); ++it) {
    if(*it == frag) {
      if(remove_file)
        (*it)->remove(err);
      delete *it;
      m_fragments.erase(it);
      break;
    }
  }
}

void Fragments::remove(int &err) {
  stopping = true;
  {
    std::unique_lock lock_wait(m_mutex);
    m_deleting = true;
    if(m_commiting || m_compacting) {
      m_cv.wait(lock_wait, [this]{ return !m_commiting && !m_compacting; });
    }
  }
  std::scoped_lock lock(m_mutex);
  for(auto frag : m_fragments) {
    frag->remove(err);
    delete frag;
  }
  m_fragments.clear();
  range = nullptr;
}

void Fragments::unload() {
  {
    std::unique_lock lock_wait(m_mutex);
    if(m_commiting || m_compacting)
      m_cv.wait(lock_wait, [this]{ return !m_commiting && !m_compacting; });
  }
  stopping = true;
  std::scoped_lock lock(m_mutex);
  for(auto frag : m_fragments)
    delete frag;
  m_fragments.clear();
  range = nullptr;
}

void Fragments::take_ownership(int &err, Fragment::Ptr take_frag) {
  auto frag = Fragment::make(
    get_log_fragment(take_frag->ts), 
    range->cfg->key_seq
  );
  Env::FsInterface::interface()->rename(
    err, 
    take_frag->get_filepath(), 
    frag->get_filepath()
  );
  if(!err) {
    frag->load_header(true);
    if(!(err = frag->error())) {
      std::scoped_lock lock(m_mutex);
      m_fragments.push_back(frag);
    }
  }
}

bool Fragments::deleting() {
  std::shared_lock lock(m_mutex);
  return m_deleting;
}

size_t Fragments::cells_count(bool only_current) {
  size_t count = 0;
  {
    std::shared_lock lock(m_mutex_cells);
    count += m_cells.size();
  }
  if(!only_current) {
    std::shared_lock lock(m_mutex);
    for(auto frag : m_fragments)
      count += frag->cells_count;
  }
  return count;
}

size_t Fragments::size() {
  std::shared_lock lock(m_mutex);
  return m_fragments.size()+1;
}

size_t Fragments::size_bytes(bool only_loaded) {
  return _size_bytes(only_loaded);
}

size_t Fragments::size_bytes_encoded() {
  std::shared_lock lock(m_mutex);
  size_t size = 0;
  for(auto frag : m_fragments)
    size += frag->size_bytes_encoded();
  return size;
}

bool Fragments::processing() {
  std::shared_lock lock(m_mutex);
  return _processing();
}

std::string Fragments::to_string() {
  size_t count = cells_count();
  std::shared_lock lock(m_mutex);

  std::string s("CommitLog(count=");
  s.append(std::to_string(count));
  
  s.append(" cells=");
  {
    std::shared_lock lock(m_mutex_cells);
  s.append(std::to_string(m_cells.size()));
  }

  s.append(" fragments=");
  s.append(std::to_string(m_fragments.size()));

  s.append(" [");
  for(auto frag : m_fragments){
    s.append(frag->to_string());
    s.append(", ");
  }
  s.append("]");

  s.append(" processing=");
  s.append(std::to_string(_processing()));

  s.append(" used/actual=");
  s.append(std::to_string(_size_bytes(true)));
  s.append("/");
  s.append(std::to_string(_size_bytes()));

  s.append(")");
  return s;
}


bool Fragments::_need_roll() const {
  auto ratio = range->cfg->log_rollout_ratio();
  auto bytes = range->cfg->block_size();
  auto cells = range->cfg->block_cells();
  return (m_cells.size() >= cells || m_cells.size_bytes() >= bytes) && 
         (m_cells.size_bytes() >= bytes * ratio ||
          m_cells.size() >= cells * ratio ||
          Env::Resources.need_ram(bytes) );
}

size_t Fragments::_need_compact(uint32_t vol, 
                std::vector<std::vector<Fragment::Ptr>>& groups) {
  if(m_fragments.size() < 3)
    return 0;

  auto fragments = m_fragments;
  std::sort(fragments.begin(), fragments.end(),
    [seq=m_cells.key_seq]
    (const Fragment::Ptr& f1, const Fragment::Ptr& f2) {
    return DB::KeySeq::compare(seq, 
      f1->interval.key_begin, f2->interval.key_begin) == Condition::GT; 
    }
  );
  size_t need = 0;
  groups.emplace_back();
  //bool need_nxt = false;
  bool add;
  auto it = fragments.begin();
  for(auto it_nxt = it+1; it_nxt<fragments.end(); ++it, ++it_nxt) {
    add = DB::KeySeq::compare(m_cells.key_seq, 
            (*it)->interval.key_end, (*it_nxt)->interval.key_begin)
              == Condition::LT;
    /*
    if(!add) for(auto frag : groups.back()) {
      if(add = DB::KeySeq::compare(m_cells.key_seq, 
                frag->interval.key_end, (*it)->interval.key_begin)
                  == Condition::LT)
        break;
    }
    */
    if(add) {
      if(groups.back().empty() || groups.back().back() != *it) {
        groups.back().push_back(*it);
        ++need;
      }
      groups.back().push_back(*it_nxt);
      ++need;
    //  need_nxt = true;
      if(groups.back().size() >= vol)
        groups.emplace_back();

    //} else if(need_nxt) {
    //  need_nxt = false;

    } else if(!groups.back().empty()) {
      if(groups.back().size() < MIN_COMPACT) {
        need -= groups.back().size();
        groups.back().clear();
      } else {
        groups.emplace_back();
      }
    }
  }
  for(auto it=groups.begin(); it < groups.end(); ) {
    if(it->size() < MIN_COMPACT) {
      need -= it->size();
      it->clear();
      groups.erase(it);
    } else {
      ++it;
    }
  }
  return need;
}

bool Fragments::_processing() const {
  if(m_commiting)
    return true;
  for(auto frag : m_fragments)
    if(frag->processing())
      return true;
  return false;
}

size_t Fragments::_size_bytes(bool only_loaded) {
  size_t size = 0;
  {
    std::shared_lock lock(m_mutex_cells);
    size += m_cells.size_bytes();
  }
  for(auto frag : m_fragments)
    size += frag->size_bytes(only_loaded);
  return size;
}



}}} // namespace SWC::Ranger::CommitLog