/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#ifndef swcdb_db_cells_MutableVec_h
#define swcdb_db_cells_MutableVec_h


#include "swcdb/core/Compat.h"
#include "swcdb/core/Buffer.h"
#include "swcdb/db/Cells/Mutable.h"


namespace SWC { namespace DB { namespace Cells {



class MutableVec final : private std::vector<Mutable*> {

  typedef std::vector<Mutable*> Vec;

  public:

  using Vec::begin;
  using Vec::end;

  const Types::KeySeq key_seq;
  uint32_t            split_size;
  uint32_t            max_revs;
  uint64_t            ttl;
  Types::Column       type;

  explicit MutableVec(const Types::KeySeq key_seq, uint32_t split_size=100000,
                      const uint32_t max_revs=1, const uint64_t ttl_ns=0,
                      const Types::Column type=Types::Column::PLAIN) noexcept
                      : key_seq(key_seq), split_size(split_size),
                        max_revs(max_revs), ttl(ttl_ns), type(type) {
  }

  MutableVec(const MutableVec&) = delete;

  MutableVec(const MutableVec&&) = delete;

  MutableVec& operator=(const MutableVec&) = delete;

  ~MutableVec();

  void free();

  void configure(uint32_t split,
                 const uint32_t revs=1, const uint64_t ttl_ns=0,
                 const Types::Column typ=Types::Column::PLAIN) noexcept;

  bool empty() const noexcept {
    return Vec::empty();
  }

  size_t size() const noexcept;

  size_t size_bytes() const noexcept;

  size_t size_of_internal() const noexcept;

  bool split(Mutable& cells, iterator it);

  void add_sorted(const Cell& cell);

  void add_raw(const Cell& cell);

  void add_raw(const Cell& cell, size_t* offset_itp, size_t* offsetp);

  void write_and_free(DynamicBuffer& cells, uint32_t& cell_count,
                      Interval& intval, uint32_t threshold,
                      uint32_t max_cells);

};


}}}



#ifdef SWC_IMPL_SOURCE
#include "swcdb/db/Cells/MutableVec.cc"
#endif


#endif // swcdb_db_Cells_MutableVec_h
