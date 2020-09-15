/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#include "swcdb/db/Cells/SpecsValue.h"
#include "swcdb/core/Serialization.h"


namespace SWC { namespace DB { namespace Specs {

Value::Value(bool own)
              : own(own), data(0), size(0), comp(Condition::NONE) {
}

Value::Value(const char* data_n, Condition::Comp comp_n, bool owner) 
            : own(false) {
  set((uint8_t*)data_n, strlen(data_n), comp_n, owner);
}

Value::Value(const char* data_n, const uint32_t size_n, 
             Condition::Comp comp_n, bool owner) 
            : own(false) {
  set((uint8_t*)data_n, size_n, comp_n, owner);
}

Value::Value(const uint8_t* data_n, const uint32_t size_n, 
             Condition::Comp comp_n, bool owner) 
            : own(false) {
  set(data_n, size_n, comp_n, owner);
}

Value::Value(int64_t count, Condition::Comp comp_n) 
            : own(false) {
  set_counter(count, comp_n);
}

Value::Value(const Value &other) 
            : own(false) {
  copy(other);
}

void Value::set_counter(int64_t count, Condition::Comp comp_n) {
  uint32_t len = Serialization::encoded_length_vi64(count);
  uint8_t data_n[len];
  uint8_t* ptr = data_n;
  Serialization::encode_vi64(&ptr, count);
  set(data_n, len, comp_n, true);
}

void Value::set(const char* data_n, Condition::Comp comp_n, bool owner) {
  set((uint8_t*)data_n, strlen(data_n), comp_n, owner);
}

void Value::set(const std::string& data_n, Condition::Comp comp_n) {
  set((uint8_t*)data_n.data(), data_n.length(), comp_n, true);
}

void Value::copy(const Value &other) {
  free(); 
  set(other.data, other.size, other.comp, true);
}

void Value::set(const uint8_t* data_n, const uint32_t size_n, 
                Condition::Comp comp_n, bool owner) {
  free();
  own   = owner;
  comp = comp_n;
  if((size = size_n))
    data = own ? (uint8_t*)memcpy(new uint8_t[size], data_n, size) 
               : (uint8_t*)data_n;
}

Value::~Value() {
  if(own && data) 
    delete [] data;
}

void Value::free() {
  if(own && data) 
    delete [] data;
  data = 0;
  size = 0;
}

bool Value::empty() const {
  return comp == Condition::NONE;
}

bool Value::equal(const Value &other) const {
  return size == other.size 
    && ((!data && !other.data) || memcmp(data, other.data, size) == 0);
}

size_t Value::encoded_length() const {
  return 1+(
    comp==Condition::NONE? 0: Serialization::encoded_length_vi32(size)+size);
}

void Value::encode(uint8_t** bufp) const {
  Serialization::encode_i8(bufp, (uint8_t)comp);
  if(comp != Condition::NONE) {
    Serialization::encode_vi32(bufp, size);
    memcpy(*bufp, data, size);
    *bufp+=size;
  }
}

void Value::decode(const uint8_t** bufp, size_t* remainp) {
  own = false;
  comp = (Condition::Comp)Serialization::decode_i8(bufp, remainp);
  if(comp != Condition::NONE){
    size = Serialization::decode_vi32(bufp, remainp);
    data = (uint8_t*)*bufp;
    *remainp -= size;
    *bufp += size;
  }
}

bool Value::is_matching(const uint8_t *other_data, 
                        const uint32_t other_size) const {
  return Condition::is_matching_extended(
    comp, data, size, other_data, other_size);
}

bool Value::is_matching(int64_t other) const {
  errno = 0;    
  char *last = (char*)data + size;
  int64_t value = strtoll((const char*)data, &last, 0); // ?cls-storage
    return Condition::is_matching(comp, value, other);
}

std::string Value::to_string() const {
  std::stringstream ss;
  print(ss);
  return ss.str();
}

void Value::print(std::ostream& out) const {
  display(out << "Value(");
  out << ')';
}

void Value::display(std::ostream& out, bool pretty) const {
  out << "size=" << size << ' ' << Condition::to_string(comp, true)
      << '"';
  if(size) {
    char hex[5];
    hex[4] = '\0';
    const uint8_t* end = data + size;
    for(const uint8_t* ptr = data; ptr < end; ++ptr) {
      if(*ptr == '"')
        out << '\\';
      if(!pretty || (31 < *ptr && *ptr < 127)) {
        out << *ptr;
      } else {
        sprintf(hex, "0x%X", *ptr);
        out << hex;
      }
    }
  }  
  out << '"';
}

}}}
