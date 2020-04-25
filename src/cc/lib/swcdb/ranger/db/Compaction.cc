/*
 * Copyright Since 2019 SWC-DB© [author: Kashirin Alex kashirin.alex@gmail.com]
 */



#include "swcdb/ranger/db/Compaction.h"
#include "swcdb/ranger/db/CompactRange.h"


namespace SWC { namespace Ranger {


Compaction::Compaction() 
          : m_check_timer(
              asio::high_resolution_timer(
                *RangerEnv::maintenance_io()->ptr())),
            m_run(true), m_running(0), m_scheduled(false),
            m_idx_cid(0), m_idx_rid(0), 
            cfg_read_ahead(
              Env::Config::settings()->get<Property::V_GUINT8>(
                "swc.rgr.compaction.read.ahead")),
            cfg_max_range(
              Env::Config::settings()->get<Property::V_GUINT8>(
                "swc.rgr.compaction.range.max")), 
            cfg_check_interval(
              Env::Config::settings()->get<Property::V_GINT32>(
                "swc.rgr.compaction.check.interval")) {
}

Compaction::~Compaction() { }
 
Compaction::Ptr Compaction::ptr() {
  return this;
}

void Compaction::stop() {
  {
    std::lock_guard lock(m_mutex);
    m_run = false;
    m_check_timer.cancel();
  }
  std::unique_lock lock_wait(m_mutex);
  if(m_running) 
    m_cv.wait(lock_wait, [&running=m_running](){return !running;});  
}

void Compaction::schedule() {
  schedule(cfg_check_interval->get());
}

void Compaction::schedule(uint32_t t_ms) {
  std::lock_guard lock(m_mutex);
  _schedule(t_ms);
}

bool Compaction::stopped() {
  std::lock_guard lock(m_mutex);
  return !m_run;
}

void Compaction::run(bool continuing) {
  {
    std::lock_guard lock(m_mutex); 
    if(!m_run || (!continuing && m_scheduled))
      return;
    m_scheduled = true;
  }

  Column::Ptr col = nullptr;
  RangePtr range  = nullptr;
  size_t ram = 0;
  for(;;) {
    
    if((col = RangerEnv::columns()->get_next(m_idx_cid)) == nullptr)
      break;
    if(col->removing()){
      ++m_idx_cid;
      continue;
    }

    if((range = col->get_next(m_idx_rid)) == nullptr) {
      ++m_idx_cid;
      continue;
    }
    ++m_idx_rid;
    if(!range->compact_possible())
      continue;

    if(!m_run)
      break;
    {
      std::lock_guard lock(m_mutex); 
      ++m_running;
    }
    asio::post(*RangerEnv::maintenance_io()->ptr(), 
      [this, range](){ compact(range); } );
    
    {
      std::lock_guard lock(m_mutex); 
      if(m_running == cfg_max_range->get())
        return;
    }
  }
  
  {
    std::lock_guard lock(m_mutex); 
    if(m_running)
      return;
  }
  compacted();
}

void Compaction::compact(RangePtr range) {

  if(!range->is_loaded() || !m_run)
    return compacted(range);

  auto& commitlog  = range->blocks.commitlog;

  uint32_t cs_size = range->cfg->cellstore_size(); 
  uint32_t blk_size = range->cfg->block_size();
  uint8_t perc = range->cfg->compact_percent(); 
  uint32_t allow_sz = (cs_size  / 100) * perc; 
  uint32_t cell_revs = range->cfg->cell_versions();
  uint64_t cell_ttl = range->cfg->cell_ttl();

  size_t value;
  bool do_compaction = false;
  std::string need;

  uint8_t compact_type = range->compact_required();
  if(compact_type == Range::COMPACT_TYPE_MAJOR) {
    do_compaction = commitlog.cells_count();
    if(!do_compaction)
      compact_type = Range::COMPACT_TYPE_NONE;
  }

  if(do_compaction) {
    need.append("Required");

  } else if(do_compaction = (value = commitlog.size_bytes(true)) >= allow_sz) {
    need.append("LogBytes=");
    need.append(std::to_string(value-allow_sz));
    compact_type = Range::COMPACT_TYPE_MAJOR;

  } else if(do_compaction = (value = commitlog.size()) >= cs_size/blk_size) {
    need.append("LogCount=");
    need.append(std::to_string(value-cs_size/blk_size));
    compact_type = Range::COMPACT_TYPE_MAJOR;

  } else if(do_compaction = range->blocks.cellstores.need_compaction(
                    cs_size + allow_sz,  blk_size + (blk_size / 100) * perc)) {
    need.append("CsResize");
    compact_type = Range::COMPACT_TYPE_MAJOR;

  } else if(do_compaction = cell_ttl && 
            (int64_t)(value = range->blocks.cellstores.get_ts_earliest())
            != DB::Cells::AUTO_ASSIGN && 
            (int64_t)value < Time::now_ns()-cell_ttl*100) {
    need.append("CsTTL");
    compact_type = Range::COMPACT_TYPE_MAJOR;

  } else if(do_compaction = range->blocks.cellstores.get_cell_revs() 
                              > cell_revs) {
    need.append("CsVersions");
    compact_type = Range::COMPACT_TYPE_MAJOR;
  }

  if(!m_run || (!do_compaction && compact_type == Range::COMPACT_TYPE_NONE))
    return compacted(range);
    
  SWC_LOGF(
    LOG_INFO, 
    "COMPACT-STARTED %d/%d %s (%s)", 
    range->cfg->cid, range->rid,
    need.c_str(),
    compact_type == Range::COMPACT_TYPE_MAJOR ? "MAJOR" : "MINOR"
  );

  auto req = std::make_shared<CompactRange>(
    ptr(),
    range, 
    cs_size,
    blk_size
  );
  req->initialize(compact_type);
  if(compact_type == Range::COMPACT_TYPE_MINOR)
    return compacted(range);

  range->scan_internal(req);
}


void Compaction::compacted(RangePtr range, bool all) {
  if(all) {
    range->blocks.release(0);
    if(range->blocks.size())
      SWC_LOGF(LOG_ERROR, "%s", range->to_string().c_str());
    SWC_ASSERT(!range->blocks.size());

  } else if(size_t bytes = Env::Resources.need_ram()) {
    range->blocks.release(bytes);
  }
  
  range->compacting(Range::COMPACT_NONE);
  compacted();
}

void Compaction::compacted() {
  std::lock_guard lock(m_mutex);

  if(m_running && m_running-- == cfg_max_range->get()) {
    asio::post(*RangerEnv::maintenance_io()->ptr(), [this](){ run(true); });
    return;
  } 
  if(!m_running) {
    if(!m_run) {
      m_cv.notify_all();
      return;
    }
    m_scheduled = false;
    _schedule(cfg_check_interval->get());
  }
}

void Compaction::_schedule(uint32_t t_ms) {
  if(!m_run || m_running || m_scheduled)
    return;

  auto set_in = std::chrono::milliseconds(t_ms);
  auto set_on = m_check_timer.expires_from_now();
  if(set_on > std::chrono::milliseconds(0) && set_on < set_in)
    return;

  m_check_timer.cancel();
  m_check_timer.expires_from_now(set_in);

  m_check_timer.async_wait(
    [this](const asio::error_code ec) {
      if(ec != asio::error::operation_aborted){
        run();
      }
  }); 

  SWC_LOGF(LOG_DEBUG, "Ranger compaction scheduled in ms=%d", t_ms);
}




}}