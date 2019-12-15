/*
 * Copyright (C) 2019 SWC-DB (author: Kashirin Alex (kashirin.alex@gmail.com))
 */


#ifndef swcdb_db_Cells_Block_h
#define swcdb_db_Cells_Block_h

#include "swcdb/db/Cells/Mutable.h"
#include "swcdb/db/Cells/Interval.h"


namespace SWC { namespace DB { namespace Cells {  

class Block {
  public:
  typedef Block* Ptr;
  
  Block(const Interval& interval, const Schema::Ptr s) 
        : m_interval(interval),  
          m_cells(Mutable(0, s->cell_versions, s->cell_ttl, s->col_type)) {
  }

  Block(const Interval& interval, 
        uint32_t cell_versions, uint32_t cell_ttl, Types::Column col_type)
        : m_interval(interval),  
          m_cells(Mutable(0, cell_versions, cell_ttl, col_type)) {
  }

  virtual bool splitter(uint32_t sz) = 0;

  virtual void loaded_cellstores(int err) = 0;

  virtual void loaded_logs(int err) = 0;
  
  virtual Ptr ptr() {
    return this;
  }

  virtual ~Block() { }

  const bool is_next(const Specs::Interval& spec) {
    std::shared_lock lock(m_mutex);
    return (spec.offset_key.empty() || m_interval.is_in_end(spec.offset_key))
            && m_interval.includes(spec);
  }

  const bool includes(const Specs::Interval& spec) {
    std::shared_lock lock(m_mutex);
    return m_interval.includes(spec);
  }
  
  const bool is_consist(const Interval& intval) {
    std::shared_lock lock(m_mutex);
    return intval.consist(m_interval);
  }

  const size_t size() {
    std::shared_lock lock(m_mutex);
    return m_cells.size();
  }

  const size_t _size() {
    return m_cells.size();
  }
  
  const size_t size_bytes() {
    std::shared_lock lock(m_mutex);
    return m_cells.size_bytes() + m_cells.size() * m_cells._cell_sz;
  }
  
  void load_cells(const Mutable& cells) {
    std::scoped_lock lock(m_mutex);
    auto ts = Time::now_ns();
    size_t added = m_cells.size();
    
    if(m_cells.size())
      cells.scan(m_interval, m_cells);
    
    added = m_cells.size() - added;
    auto took = Time::now_ns()-ts;
    std::cout << "Cells::Block::load_cells(cells)"
              << " synced=0"
              << " avail=" << cells.size() 
              << " added=" << added 
              << " skipped=" << cells.size()-added
              << " avg=" << (added>0 ? took / added : 0)
              << " took=" << took
              << std::flush << " " << m_cells.to_string() << "\n";
    
  }

  size_t load_cells(const uint8_t* buf, size_t remain, 
                    size_t avail, bool& was_splitted) {
    auto ts = Time::now_ns();
    Cell cell;
    size_t count = 0;
    size_t added = 0;
    uint32_t sz = 0;
    
    const uint8_t** rbuf = &buf;
    size_t* remainp = &remain;

    std::scoped_lock lock(m_mutex);
    bool synced = !m_cells.size();
    
    while(remain) {
      try {
        cell.read(rbuf, remainp);
        count++;
      } catch(std::exception) {
        SWC_LOGF(LOG_ERROR, 
          "Cell trunclated at count=%llu remain=%llu %s, %s", 
          count, avail-count, 
          cell.to_string().c_str(),  m_interval.to_string().c_str());
        break;
      }

      if(!m_interval.key_begin.empty() 
          && m_interval.key_begin.compare(cell.key) == Condition::LT)
        continue;
      if(!m_interval.key_end.empty() 
          && m_interval.key_end.compare(cell.key) == Condition::GT)
        break;
      
      //if(!m_interval.consist(cell.key))
        //continue;

      if(synced)
        m_cells.push_back(cell);
      else
        m_cells.add(cell);
      
      added++;

      if(splitter(sz = m_cells.size()) 
        && !was_splitted && sz != m_cells.size())
        was_splitted = true;
    }
    
    auto took = Time::now_ns()-ts;
    std::cout << "Cells::Block::load_cells(rbuf)"
              << " synced=" << synced 
              << " avail=" << avail 
              << " added=" << added 
              << " skipped=" << avail-added
              << " avg=" << (added>0 ? took / added : 0)
              << " took=" << took
              << std::flush << " " << m_cells.to_string() << "\n";
             
    return added;
  }

  void free_key_begin() {
    std::scoped_lock lock(m_mutex);
    m_interval.key_begin.free();
  }

  void free_key_end() {
    std::scoped_lock lock(m_mutex);
    m_interval.key_end.free();
  }

  const std::string to_string() {
    std::shared_lock lock(m_mutex);
    std::string s("Cells::Block(");
    s.append(m_interval.to_string());
    s.append(" ");
    s.append(m_cells.to_string());
    s.append(")");
    return s;
  }
  
  protected:
  std::shared_mutex       m_mutex;
  Interval                m_interval;
  Mutable                 m_cells;
};

}}}

#endif