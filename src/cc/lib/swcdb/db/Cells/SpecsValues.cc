/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#include "swcdb/db/Cells/SpecsValues.h"
#include "swcdb/core/Serialization.h"


namespace SWC { namespace DB { namespace Specs {



Values::Values(const Values& other)
              : Vec(other), col_type(other.col_type) {
}

Values::Values(Values&& other) noexcept
               : Vec(std::move(other)), col_type(other.col_type) {
}

Values& Values::operator=(const Values& other) {
  copy(other);
  return *this;
}

Values& Values::operator=(Values&& other) noexcept {
  move(other);
  return *this;
}

void Values::copy(const Values& other) {
  Vec::operator=(other);
  col_type = other.col_type;
}

void Values::move(Values& other) noexcept {
  Vec::operator=(std::move(other));
  col_type = other.col_type;
}

Value& Values::add(Condition::Comp comp) {
  return emplace_back(true, comp);
}

Value& Values::add(Value&& other) {
  return emplace_back(std::move(other));
}

size_t Values::size_of_internal() const noexcept {
  size_t sz = sizeof(*this);
  for(const auto& value : *this)
    sz += sizeof(value) + value.size;
  return sz;
}

bool Values::equal(const Values& other) const noexcept {
  if(col_type == other.col_type && size() == other.size()) {
    auto it = cbegin();
    for(auto it2 = other.cbegin(); it != cend(); ++it, ++it2)
      if(!it->equal(*it2))
        return false;
  }
  return true;
}

bool Values::is_matching(const Cells::Cell& cell) const {
  if(empty())
    return true;

  switch(col_type) {
    case Types::Column::PLAIN: {
      for(const auto& value : *this) {
        if(!value.is_matching_plain(cell))
          return false;
      }
      return true;
    }
    case Types::Column::SERIAL: {
      for(const auto& value : *this) {
        if(!value.is_matching_serial(cell))
          return false;
      }
      return true;
    }
    case Types::Column::COUNTER_I64:
    case Types::Column::COUNTER_I32:
    case Types::Column::COUNTER_I16:
    case Types::Column::COUNTER_I8: {
      for(const auto& value : *this) {
        if(!value.is_matching_counter(cell))
          return false;
      }
      return true;
    }
    default:
      return false;
  }
}

size_t Values::encoded_length() const noexcept {
  size_t sz = 0;
  size_t c = 0;
  for(const auto& value : *this) {
    if(value.comp != Condition::NONE) {
      ++c;
      sz += value.encoded_length();
    }
  }
  return Serialization::encoded_length_vi64(c) + sz;
}

void Values::encode(uint8_t** bufp) const {
  size_t c = 0;
  for(const auto& value : *this) {
    if(value.comp != Condition::NONE)
      ++c;
  }
  Serialization::encode_vi64(bufp, c);
  for(const auto& value : *this) {
    if(value.comp != Condition::NONE)
      value.encode(bufp);
  }
}

void Values::decode(const uint8_t** bufp, size_t* remainp, bool owner) {
  clear();
  resize(Serialization::decode_vi64(bufp, remainp));
  for(auto& value : *this)
    value.decode(bufp, remainp, owner);
}

void Values::print(std::ostream& out) const {
  out << "Values(";
  if(!empty()) {
    out << "size=" << size() << " [";
    for(const auto& value : *this) {
      value.print(col_type, out);
      out << ", ";
    }
    out << ']';
  }
  out << ')';
}

void Values::display(std::ostream& out, bool pretty,
                     const std::string& offset) const {
  out << offset << "Values([\n";
  for(const auto& value : *this) {
    value.display(col_type, out << offset << " Value(", pretty);
    out << ")\n";
  }
  out << offset << "])\n";
}


}}}
