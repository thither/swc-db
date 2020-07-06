/*
 * Copyright Since 2019 SWC-DB© [author: Kashirin Alex kashirin.alex@gmail.com]
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#ifndef swc_manager_db_Column_h
#define swc_manager_db_Column_h

#include "swcdb/manager/db/Range.h"

#include <memory>
#include <unordered_map>

namespace SWC { namespace Manager {


class Column final : private std::vector<Range::Ptr> {
  
  public:

  enum State {
    OK,
    LOADING,
    DELETED
  };
  typedef std::shared_ptr<Column> Ptr;
  
  static bool create(int &err, const cid_t cid) {
    Env::FsInterface::interface()->mkdirs(
      err, DB::RangeBase::get_column_path(cid));
    return true;
  }

  static bool remove(int &err, const cid_t cid) {
    Env::FsInterface::interface()->rmdir_incl_opt_subs(
      err, 
      DB::RangeBase::get_column_path(cid), 
      DB::RangeBase::get_column_path()
    );
    return true;
  }

  const ColumnCfg  cfg;

  Column(const DB::Schema::Ptr& schema) 
        : cfg(schema), m_state(State::LOADING) {
  }

  ~Column() { }

  void init(int &err) {
    FS::IdEntries_t entries;

    {
      std::scoped_lock lock(m_mutex);

      if(!exists_range_path(err))
        create_range_path(err);
      else
        ranges_by_fs(err, entries); 
    }

    if(entries.empty())
      entries.push_back(1); // initialize 1st range

    for(auto rid : entries)
      get_range(err, rid, true); 
  }

  void set_loading() {
    std::scoped_lock lock(m_mutex);
    _set_loading();
  }

  void state(int& err) {
    std::shared_lock lock(m_mutex);
    if(m_state == State::OK)
      return;
    if(m_state == State::DELETED)
      err = Error::COLUMN_MARKED_REMOVED;
    else
      err = Error::COLUMN_NOT_READY;
  }
  
  Range::Ptr get_range(int&, const rid_t rid, bool initialize=false) {
    std::scoped_lock lock(m_mutex);

    auto it = std::find_if(begin(), end(), [rid](const Range::Ptr& range) 
                                           {return range->rid == rid;});
    if(it != end())
      return *it;

    if(initialize)
      return emplace_back(new Range(&cfg, rid));
      
    return nullptr;
  }

  Range::Ptr get_range(int&, 
                       const DB::Cell::Key& range_begin, 
                       const DB::Cell::Key& range_end, 
                       bool next_range) {
    bool found = false;
    uint32_t any_is = Types::MetaColumn::is_master(cfg.cid)
      ? 2 : (Types::MetaColumn::is_meta(cfg.cid) ? 1 : 0);

    std::shared_lock lock(m_mutex);
    for(auto& range : *this) {
      if(!range->includes(range_begin, range_end, any_is)) 
        continue;
      if(!next_range)
        return range;
      if(!found) {
        found = true;
        continue;
      }
      return range;
    }
    return nullptr;
  }

  void sort(Range::Ptr& range, const DB::Cells::Interval& interval) {
    std::scoped_lock lock(m_mutex);
    
    // std::sort(begin(), end(), Range); 
    if(!range->equal(interval)) {
      range->set(interval);
      
      if(size() > 1) {
        auto it = std::find_if(
          begin(), end(), [rid=range->rid](const Range::Ptr& range)
                          {return range->rid == rid;});
        if(it != end())
          erase(it);
        bool added = false;
        for(auto it=begin(); it<end(); ++it) {
          if((*it)->after(range)) {
            insert(it, range);
            added = true;
            break;
          }
        }
        if(!added)
          push_back(range);
      }
    }
    
    apply_loaded_state();
  }

  Range::Ptr create_new_range(rgrid_t rgrid) {
    std::scoped_lock lock(m_mutex);

    auto& range = emplace_back(new Range(&cfg, _get_next_rid()));
    // if !rid - err reached id limit
    range->set_state(Range::State::CREATED, rgrid);
    return range;
  }

  rid_t get_next_rid() {
    std::shared_lock lock(m_mutex);
    return _get_next_rid();
  }

  Range::Ptr get_next_unassigned() {
    std::scoped_lock lock(m_mutex);

    for(auto& range : *this) {
      if(range->need_assign()) {
        _set_loading();
        return range;
      }
    }
    return nullptr;
  }

  void set_rgr_unassigned(rgrid_t rgrid) {
    std::scoped_lock lock(m_mutex);    

    for(auto& range : *this) {
      if(range->get_rgr_id() == rgrid) {
        range->set_state(Range::State::NOTSET, 0);
        _set_loading();
      }
    }
    _remove_rgr_schema(rgrid);
  }

  void change_rgr(rgrid_t rgrid_old, rgrid_t rgrid) {
    std::scoped_lock lock(m_mutex);

    for(auto& range : *this) {
      if(range->get_rgr_id() == rgrid_old)
        range->set_rgr_id(rgrid);
    }

    for(auto it = m_schemas_rev.begin(); it != m_schemas_rev.end(); ++it) {
      if(it->first == rgrid_old) {
        m_schemas_rev.emplace(rgrid, it->second);
        m_schemas_rev.erase(it);
        break;
      }
    }
  }

  void change_rgr_schema(const rgrid_t rgrid, int64_t rev=0) {
    std::scoped_lock lock(m_mutex);

    auto it = m_schemas_rev.find(rgrid);
    if(it == m_schemas_rev.end())
       m_schemas_rev.emplace(rgrid, rev);
    else
      it->second = rev;
  }

  void remove_rgr_schema(const rgrid_t rgrid) {
    std::scoped_lock lock(m_mutex);
    _remove_rgr_schema(rgrid);
  }

  void need_schema_sync(int64_t rev, std::vector<rgrid_t> &rgrids) {
    std::shared_lock lock(m_mutex);

    for(auto it = m_schemas_rev.begin(); it != m_schemas_rev.end(); ++it) {
      if(it->second != rev)
        rgrids.push_back(it->first);
    }
  }

  bool need_schema_sync(const rgrid_t rgrid, int64_t rev) {
    std::shared_lock lock(m_mutex);

    auto it = m_schemas_rev.find(rgrid);
    if(it != m_schemas_rev.end())
      return rev != it->second;
    return true;
  }
  
  void assigned(std::vector<rgrid_t> &rgrids) {
    std::shared_lock lock(m_mutex);

    rgrid_t rgrid;
    for(auto& range : *this) {
      if(!(rgrid = range->get_rgr_id()))
        continue;
      if(std::find_if(rgrids.begin(), rgrids.end(), 
         [rgrid](const rgrid_t& rgrid2)
         {return rgrid == rgrid2;}) == rgrids.end())
        rgrids.push_back(rgrid);
    }
  }
  
  void remove_range(rid_t rid) {
    std::scoped_lock lock(m_mutex);

    if(!size()) 
      return;
    auto it = std::find_if(begin(), end(), [rid](const Range::Ptr& range) 
                                           {return range->rid == rid;});
    if(it == end())
      return;
    (*it)->set_deleted();
    erase(it);
    apply_loaded_state();
  }

  bool do_remove() {
    std::scoped_lock lock(m_mutex);

    bool was = m_state == State::DELETED;
    m_state = State::DELETED;
    m_schemas_rev.clear();
    
    if(!was) {
      for(auto& range : *this)
        range->set_deleted();
    }
    return !was;
  }

  bool finalize_remove(int &err, rgrid_t rgrid=0) {
    std::scoped_lock lock(m_mutex);
    
    if(!rgrid) {
      clear();
    } else {
      rgrid_t eid;
      for(auto it=begin(); it<end(); ) {
        if((eid = (*it)->get_rgr_id()) == rgrid || !eid)
          erase(it);
        else
          ++it;
      }
    }
    if(empty()) {
      Env::FsInterface::interface()->rmdir(
        err, DB::RangeBase::get_path(cfg.cid));
      SWC_LOGF(LOG_DEBUG, "FINALIZED REMOVE %s", _to_string().c_str());
      return true;
    }
    return false;
  }

  std::string to_string() {
    std::shared_lock lock(m_mutex);
    return _to_string();
  }

  private:
  
  bool exists(int &err) {
    return Env::FsInterface::interface()->exists(
      err, DB::RangeBase::get_column_path(cfg.cid));
  }

  bool exists_range_path(int &err) {
    return Env::FsInterface::interface()->exists(
      err, DB::RangeBase::get_path(cfg.cid));
  }

  void create_range_path(int &err) {
    Env::FsInterface::interface()->mkdirs(
      err, DB::RangeBase::get_path(cfg.cid));
  }

  void ranges_by_fs(int &err, FS::IdEntries_t &entries) {
    Env::FsInterface::interface()->get_structured_ids(
      err, DB::RangeBase::get_path(cfg.cid), entries);
  }

  std::string _to_string() {
    std::string s("[");
    s.append(cfg.to_string());
    s.append(", next-rid=");
    s.append(std::to_string(_get_next_rid()));
    s.append(", ranges=(");
    
    for(auto& range : *this) {
      s.append(range->to_string());
      s.append(",");
    }
    s.append(")]");
    return s;
  }

  void _set_loading() {
    if(m_state == State::OK)
      m_state = State::LOADING;
  }
  
  rid_t _get_next_rid() {
    rid_t rid = 0;
    while(++rid && std::find_if(
      begin(), end(), [rid](const Range::Ptr& range)
      { return range->rid == rid; }) != end());
    return rid;
  }

  void _remove_rgr_schema(const rgrid_t rgrid) {
    auto it = m_schemas_rev.find(rgrid);
    if(it != m_schemas_rev.end())
       m_schemas_rev.erase(it);
  }

  void apply_loaded_state() {
    if(m_state != State::LOADING)
      return;

    for(auto& range : *this) {
      if(!range->assigned())
        return;
    }
    if(m_state == State::LOADING)
      m_state = State::OK;
  }

  std::shared_mutex         m_mutex;
  std::atomic<State>        m_state;

  std::unordered_map<rgrid_t, int64_t>   m_schemas_rev;

};

}}

#endif // swc_manager_db_Column_h