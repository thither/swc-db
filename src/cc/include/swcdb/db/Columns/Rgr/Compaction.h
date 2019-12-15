/*
 * Copyright (C) 2019 SWC-DB (author: Kashirin Alex (kashirin.alex@gmail.com))
 */


#ifndef swcdb_lib_db_Columns_Rgr_Compaction_h
#define swcdb_lib_db_Columns_Rgr_Compaction_h

#include "swcdb/db/Columns/Rgr/Columns.h"


namespace SWC { namespace server { namespace Rgr {

class Compaction : public std::enable_shared_from_this<Compaction> {
  public:

  typedef std::shared_ptr<Compaction> Ptr;

  Compaction(uint32_t workers=Env::Config::settings()->get<int32_t>(
                                              "swc.rgr.maintenance.handlers"))
            : m_io(std::make_shared<IoContext>("Maintenance", workers)), 
              m_check_timer(asio::high_resolution_timer(*m_io->ptr())),
              m_run(true), running(0), m_scheduled(false),
              m_idx_cid(0), m_idx_rid(0), 
              cfg_check_interval(Env::Config::settings()->get_ptr<gInt32t>(
                "swc.rgr.compaction.check.interval")), 
              cfg_cs_max(Env::Config::settings()->get_ptr<gInt32t>(
                "swc.rgr.Range.CellStore.count.max")), 
              cfg_cs_sz(Env::Config::settings()->get_ptr<gInt32t>(
                "swc.rgr.Range.CellStore.size.max")), 
              cfg_compact_percent(Env::Config::settings()->get_ptr<gInt32t>(
                "swc.rgr.Range.compaction.size.percent")) {
    m_io->run(m_io);
  }
  
  virtual ~Compaction(){}
 
  void stop() {
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_run = false;
      m_check_timer.cancel();
      m_io->stop();
    }
    std::unique_lock<std::mutex> lock_wait(m_mutex);
    if(running) 
      m_cv.wait(lock_wait, [&running=running](){return !running;});  
  }

  void schedule() {
    schedule(cfg_check_interval->get());
  }
  
  void schedule(uint32_t t_ms) {
    std::lock_guard<std::mutex> lock(m_mutex);
    _schedule(t_ms);
  }
  
  void run() {
    {
      std::lock_guard<std::mutex> lock(m_mutex); 
      if(!m_run)
        return;
      m_scheduled = true;
    }

    Column::Ptr col     = nullptr;
    Range::Ptr range  = nullptr;
    size_t ram = 0;
    for(;;) {
      
      if((col = Env::RgrColumns::get()->get_next(m_idx_cid)) == nullptr)
        break;
      if(col->removing()){
        m_idx_cid++;
        continue;
      }

      if((range = col->get_next(m_idx_rid)) == nullptr) {
        m_idx_cid++;
        continue;
      }
      m_idx_rid++;
      if(!range->is_loaded() 
        || range->compacting() 
        || range->blocks.processing())
        continue;
        
      range->compacting(Range::Compact::CHECKING);
      ++running;
      asio::post(
        *m_io->ptr(), 
        [range, ptr=shared_from_this()](){ 
          ptr->compact(range); 
        }
      );
      if(running == m_io->get_size())
        return;
    }
    
    if(!running)
      compacted();
  }

  void compact(Range::Ptr range) {

    if(!range->is_loaded())
      return compacted(range);

    auto& commitlog  = range->blocks.commitlog;
    auto& cellstores = range->blocks.cellstores;

    uint32_t cs_size = cfg_cs_sz->get(); 

    uint32_t blk_size = range->cfg->block_size();
    uint32_t blk_cells = range->cfg->block_cells();
    Types::Encoding blk_encoding = range->cfg->block_enc();

    uint32_t perc     = cfg_compact_percent->get(); 
    // % of size of either by cellstore or block
    
    uint32_t allow_sz = (cs_size  / 100) * perc; 
    uint32_t allowed_sz_cs  = cs_size + allow_sz;
    uint32_t log_sz = commitlog.size_bytes();

    bool do_compaction = log_sz >= allowed_sz_cs
      || commitlog.size() > allowed_sz_cs/blk_size 
      || (log_sz > allow_sz && log_sz > (cellstores.size_bytes()/100) * perc)
      || cellstores.need_compaction(
          allowed_sz_cs,  blk_size + (blk_size / 100) * perc);
    
    SWC_LOGF(LOG_INFO, 
      "Compaction %s-range %d/%d allow=%dMB"
      " log=%d=%d/%dMB cs=%d=%d/%dMB blocks=%d=%dMB",
      do_compaction ? "started" : "skipped",
      range->cfg->cid, range->rid, 
      allowed_sz_cs/1000000,

      commitlog.size(),
      commitlog.size_bytes(true)/1000000,
      log_sz/1000000,

      cellstores.size(),
      cellstores.size_bytes(true)/1000000,
      cellstores.size_bytes()/1000000,
      
      range->blocks.size(),
      range->blocks.size_bytes()/1000000
    );

    if(!do_compaction)
      return compacted(range);
      
    range->compacting(Range::Compact::COMPACTING);
    range->blocks.wait_processing();
    
    auto req = std::make_shared<CompactScan>(
      shared_from_this(),
      range, 
      cs_size, 
      blk_size, blk_cells, blk_encoding, 
      range->cfg->cell_versions, 
      range->cfg->cell_ttl, 
      range->cfg->col_type
    );
    commitlog.commit_new_fragment(true);
    commitlog.get(req->fragments_old); // fragments for deletion at finalize-compaction 

    //std::cout << "\n\n" << range->to_string() << "\n\n";
    range->scan_internal(req);
  }

  class CompactScan : public DB::Cells::ReqScan {
    public:
  
    typedef std::shared_ptr<CompactScan>          Ptr;
    std::vector<Files::CommitLog::Fragment::Ptr>  fragments_old;
    std::atomic<size_t> total_cells = 0;

    CompactScan(Compaction::Ptr compactor, Range::Ptr range,
                const uint32_t cs_size, 
                const uint32_t blk_size, const uint32_t blk_cells, 
                const Types::Encoding blk_encoding,
                uint32_t cell_versions, uint32_t cell_ttl, Types::Column col_type) 
                : compactor(compactor), range(range),
                  cs_size(cs_size), blk_size(blk_size), blk_cells(blk_cells), 
                  blk_encoding(blk_encoding),
                  cell_versions(cell_versions),
                  cell_ttl(cell_ttl),
                  col_type(col_type) {
      make_cells();
    }

    virtual ~CompactScan() { }

    Ptr shared() {
      return std::dynamic_pointer_cast<CompactScan>(shared_from_this());
    }

    void make_cells() {
      cells.reset(
        1000, 
        cell_versions, 
        cell_ttl, 
        col_type
      );
    }

    bool reached_limits() override {
      return m_stopped 
          || cells.size_bytes() >= blk_size 
          || cells.size() >= blk_cells;
    }

    void response(int &err) override {
      if(m_stopped || !ready(err))
        return;
      /*
      std::cout << "CompactScan::response: \n"
                << "  spec=" << spec.to_string() << "\n"
                << " cells="<< cells.size() << " sz=" << cells.size_bytes() << "\n";
      */
      total_cells += cells.size();
      std::cout << "CompactScan::response: total_cells=" << total_cells.load() << "\n";

      {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(!cells.size()) {
          if(m_writing)
            m_queue.push(nullptr);
          else 
            finalize();
          return;
        }
        m_getting = false;
      }

      auto selected_cells = new DB::Cells::Mutable(cells);
      make_cells();

      DB::Cells::Cell first;
      selected_cells->get(0, first);
      DB::Cells::Cell last;
      selected_cells->get(-1, last);
      /*
      std::cout << "CompactScan::response : \n" 
                << " first=" << first.to_string() << "\n"
                << "  last=" << last.to_string() << "\n";
      */
      spec.offset_key.copy(last.key);
      spec.offset_rev = last.timestamp;
      
      { 
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(selected_cells);
        if(m_writing) 
          return;
        m_writing = true;
      }
      asio::post(*compactor->io()->ptr(), [ptr=shared()](){ ptr->process(); });
      request_more();
    }

    private:

    void request_more() {
      {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(m_getting 
          || (!m_queue.empty() 
              && (m_queue.size() >= compactor->io()->get_size()
                  || m_queue.size() > Env::Resources.avail_ram()/blk_size )))
          return;
        m_getting = true;
      }

      asio::post(
        *compactor->io()->ptr(), 
        [ptr=shared()](){
          ptr->range->scan_internal(ptr->get_req_scan());
        }
      );
    }

    void process() {
      int err = Error::OK;
      DB::Cells::Mutable* selected_cells;
      for(;;) {
        {
          std::lock_guard<std::mutex> lock(m_mutex);
          selected_cells = m_queue.front();
        }
        if(selected_cells == nullptr) {
          finalize();
          return;
        }
        if(m_stopped)
          return;
        
        request_more();

        write_cells(err, selected_cells);
        delete selected_cells;

        if(m_stopped)
          return;
        if(err || !range->is_loaded())
          return quit();

        {
          std::lock_guard<std::mutex> lock(m_mutex);
          m_queue.pop();
          m_writing = !m_queue.empty();
          if(!m_writing)
            break;
        }
      }
      request_more();
    }

    uint32_t create_cs(int& err) { 
      if(!tmp_dir) { 
        Env::FsInterface::interface()->rmdir(
          err, range->get_path(Range::cellstores_tmp_dir));
        err = Error::OK;
        Env::FsInterface::interface()->mkdirs(
          err, range->get_path(Range::cellstores_tmp_dir));
        tmp_dir = true;
        if(err)
          return 0;
      }

      uint32_t id = cellstores.size()+1;
      cs_writer = std::make_shared<Files::CellStore::Write>(
        id, 
        range->get_path_cs_on(Range::cellstores_tmp_dir, id), 
        blk_encoding
      );
      int32_t replication=-1;
      cs_writer->create(err, -1, replication, blk_size);
      return id;

    }

    void write_cells(int& err, DB::Cells::Mutable* selected_cells) {
      
      if(cs_writer == nullptr) {
        uint32_t id = create_cs(err);
        if(err)
          return;

        if(id == 1 && range->is_any_begin()) {
          DB::Cells::Interval blk_intval;
          DynamicBuffer buff;
          uint32_t cell_count = 0;
          selected_cells->write_and_free(buff, cell_count, blk_intval, 0, 1);
          // 1st block of begin-any set with key_end as first cell
          blk_intval.key_begin.free();

          if(range->is_any_end() && !selected_cells->size()) // there was one cell
            blk_intval.key_end.free(); 
          
          cs_writer->block(err, blk_intval, buff, cell_count);

          if(err || !selected_cells->size())
            return;
        }
      }

      DB::Cells::Interval blk_intval;
      DynamicBuffer buff;
      uint32_t cell_count = 0;

      if(range->is_any_end()){
        if(!last_cell.key.empty()) {
          cell_count++;
          last_cell.write(buff);
          blk_intval.expand(last_cell);
        }
        selected_cells->pop(-1, last_cell);
        //last block of end-any to be set with first key as last cell
      }
      selected_cells->write_and_free(buff, cell_count, blk_intval, 0, 0);

      if(buff.fill()) {
        cs_writer->block(err, blk_intval, buff, cell_count);
        if(err)
          return;
      }
      if(cs_writer->size >= cs_size) {
        add_cs(err);
        if(err)
          return;
      }
    }

    void add_cs(int& err) {
      cs_writer->finalize(err);
      cellstores.push_back(cs_writer);
      cs_writer = nullptr;
    }

    void finalize() {
      
      if(!range->is_loaded())
        return quit();

      int err = Error::OK;

      bool empty_cs = false;
      if(cs_writer != nullptr) {
        if(range->is_any_end() && !last_cell.key.empty()) {
          DB::Cells::Interval blk_intval;
          blk_intval.expand(last_cell);
          blk_intval.key_end.free();

          uint32_t cell_count = 1;
          DynamicBuffer buff;
          last_cell.write(buff);
          last_cell.free();
          // last block of end-any set with key_begin as last cell
          cs_writer->block(err, blk_intval, buff, cell_count);
        }
      } else {
        uint32_t id = create_cs(err);
        if(err)
          return;
        // as an initial empty range cs
        uint32_t cell_count = 0;
        DB::Cells::Interval blk_intval;
        range->get_interval(blk_intval.key_begin, blk_intval.key_end);
        DynamicBuffer buff;
        cs_writer->block(err, blk_intval, buff, cell_count);
        empty_cs = true;
      }
      if(err || !cs_writer->size)
        return quit();

      add_cs(err);
      if(err)
        return quit();
      
      //std::cout << "CellStore::Writers: \n";
      //for(auto cs : cellstores)
      //  std::cout << " " << cs->to_string() << "\n";

      if(cellstores.size()) 
        range->apply_new(err, cellstores, fragments_old);
      if(err)
        return quit();
        
      compactor->compacted(range, empty_cs);

      std::cout << "Compact ::finalized\n";
    }

    void quit() {
      m_stopped = true;
      int err = Error::OK;
      if(cs_writer != nullptr) {
        cs_writer->remove(err);
        cs_writer = nullptr;
      }
      if(tmp_dir) {
        tmp_dir = false;
        Env::FsInterface::interface()->rmdir(
          err, range->get_path(Range::cellstores_tmp_dir));
      }

      compactor->compacted(range);
    }


    Compaction::Ptr         compactor;
    Range::Ptr              range;
    const uint32_t          cs_size;
    const uint32_t          blk_size;
    const uint32_t          blk_cells;
    const Types::Encoding   blk_encoding;
    const uint32_t          cell_versions;
    const uint32_t          cell_ttl; 
    const Types::Column     col_type;
    
    bool                            tmp_dir = false;
    Files::CellStore::Write::Ptr    cs_writer = nullptr;
    Files::CellStore::Writers       cellstores;
    DB::Cells::Cell                 last_cell;

    std::mutex                      m_mutex;
    bool                            m_writing = false;
    std::queue<DB::Cells::Mutable*> m_queue;
    std::atomic<bool>               m_stopped = false;
    bool                            m_getting = false;
  };

  IoContext::Ptr io() {
    return m_io;
  }

  private:
  
  void compacted(Range::Ptr range, bool all=false) {
    if(all)
      range->blocks.release(0);
    else if(size_t bytes = Env::Resources.need_ram())
      range->blocks.release(bytes);
    
    range->compacting(Range::Compact::NONE);
    compacted();
  }

  void compacted() {
    //std::cout << "Compaction::compacted running=" << running.load() << "\n";

    if(running && running-- == m_io->get_size()) {
      run();
      
    } else if(!running) {
      std::lock_guard<std::mutex> lock(m_mutex); 
      m_scheduled = false;
      _schedule(cfg_check_interval->get());
    }

    if(!m_run) {
      m_cv.notify_all();
      return;
    }
  }

  void _schedule(uint32_t t_ms = 300000) {
    if(!m_run || running || m_scheduled)
      return;

    auto set_in = std::chrono::milliseconds(t_ms);
    auto set_on = m_check_timer.expires_from_now();
    if(set_on > std::chrono::milliseconds(0) && set_on < set_in)
      return;

    m_check_timer.cancel();
    m_check_timer.expires_from_now(set_in);

    m_check_timer.async_wait(
      [ptr=shared_from_this()](const asio::error_code ec) {
        if (ec != asio::error::operation_aborted){
          ptr->run();
        }
    }); 

    SWC_LOGF(LOG_DEBUG, "Ranger compaction scheduled in ms=%d", t_ms);
  }

  std::mutex                   m_mutex;
  IoContext::Ptr               m_io;
  asio::high_resolution_timer  m_check_timer;
  bool                         m_run;
  std::atomic<uint32_t>        running;
  bool                         m_scheduled;
  std::condition_variable      m_cv;
  
  size_t                       m_idx_cid;
  size_t                       m_idx_rid;

  const gInt32tPtr            cfg_check_interval;
  const gInt32tPtr            cfg_cs_max;
  const gInt32tPtr            cfg_cs_sz;
  const gInt32tPtr            cfg_compact_percent;
};





}}}
#endif