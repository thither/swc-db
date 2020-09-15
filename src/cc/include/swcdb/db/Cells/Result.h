/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#ifndef swcdb_db_Cells_Result_h
#define swcdb_db_Cells_Result_h

#include "swcdb/db/Cells/Cell.h"
#include "swcdb/db/Cells/Interval.h"

namespace SWC { namespace DB { namespace Cells {


class Result final : private std::vector<Cell*> {
  public:

  using std::vector<Cell*>::empty;
  using std::vector<Cell*>::capacity;
  using std::vector<Cell*>::reserve;
  using std::vector<Cell*>::size;
  using std::vector<Cell*>::back;
  using std::vector<Cell*>::front;
  using std::vector<Cell*>::begin;
  using std::vector<Cell*>::end;
  using std::vector<Cell*>::operator[];

  size_t            bytes;
  Types::Column     type;
  uint32_t          max_revs;
  uint64_t          ttl;

  explicit Result(const uint32_t max_revs=1, const uint64_t ttl_ns=0, 
                  const Types::Column type=Types::Column::PLAIN);

  explicit Result(Result& other);

  Result(const Result&& other) = delete;

  Result& operator=(const Result& other) = delete;

  ~Result();

  void free();

  void reset(const uint32_t revs=1, const uint64_t ttl_ns=0, 
             const Types::Column typ=Types::Column::PLAIN);

  void configure(const uint32_t revs=1, const uint64_t ttl_ns=0,
                 const Types::Column typ=Types::Column::PLAIN);

  size_t size_bytes() const;


  void take(Result& other);

  void add(const Cell& cell, bool no_value=false);
  
  size_t add(const uint8_t* ptr, size_t remain);


  Cell* takeout_begin(size_t idx);

  Cell* takeout_end(size_t idx);


  void write(DynamicBuffer& cells) const;

  void write_and_free(DynamicBuffer& cells, uint32_t& cell_count,
                      Interval& intval, uint32_t threshold, 
                      uint32_t max_cells);


  void print(std::ostream& out, bool with_cells=false) const;

};


}}}

#ifdef SWC_IMPL_SOURCE
#include "swcdb/db/Cells/Result.cc"
#endif 

#endif // swcdb_db_Cells_Result_h