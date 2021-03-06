/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#ifndef swcdb_db_cells_SpecsValues_h
#define swcdb_db_cells_SpecsValues_h


#include "swcdb/db/Cells/SpecsValue.h"


namespace SWC { namespace DB { namespace Specs {


class Values : public std::vector<Value> {
  public:

  typedef std::vector<Value> Vec;

  using Vec::insert;
  using Vec::emplace_back;


  Types::Column col_type;

  Values(Types::Column col_type = Types::Column::UNKNOWN) noexcept
        : col_type(col_type) {
  }

  Values(const Values& other);

  Values(Values&& other) noexcept;

  Values& operator=(const Values& other);

  Values& operator=(Values&& other) noexcept;

  void copy(const Values& other);

  void move(Values& other) noexcept;

  Value& add(Condition::Comp comp=Condition::EQ);

  Value& add(Value&& other);

  size_t size_of_internal() const noexcept;

  bool equal(const Values& other) const noexcept;

  bool is_matching(const Cells::Cell& cell) const;

  size_t encoded_length() const noexcept;

  void encode(uint8_t** bufp) const;

  void decode(const uint8_t** bufp, size_t* remainp, bool owner=false);

  void print(std::ostream& out) const;

  void display(std::ostream& out, bool pretty,
               const std::string& offset) const;

};


}}}



#ifdef SWC_IMPL_SOURCE
#include "swcdb/db/Cells/SpecsValues.cc"
#endif


#endif // swcdb_db_cells_SpecsValues_h
