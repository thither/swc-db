/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */



#include "swcdb/core/Compat.h"
#include "swcdb/db/Types/SystemColumn.h"


namespace SWC { namespace DB { namespace Types {  namespace SystemColumn {


SWC_SHOULD_INLINE
bool is_master(cid_t cid) noexcept {
  return cid <= CID_MASTER_END;
}

SWC_SHOULD_INLINE
bool is_meta(cid_t cid) noexcept {
  return cid >= CID_META_BEGIN && cid <= CID_META_END;
}

SWC_SHOULD_INLINE
bool is_data(cid_t cid) noexcept {
  return cid > CID_META_END;
}


Range get_range_type(cid_t cid) noexcept {
  if(cid <= CID_MASTER_END)
    return Range::MASTER;
  if(cid <= CID_META_END)
    return Range::META;
  return Range::DATA;
}

KeySeq get_seq_type(cid_t cid) noexcept {
  switch(cid) {
    case 2:
    case 6:
      return KeySeq::VOLUME;
    case 3:
    case 7:
      return KeySeq::FC_LEXIC;
    case 4:
    case 8:
      return KeySeq::FC_VOLUME;
    default:
      return KeySeq::LEXIC;
  }
}


cid_t get_master_cid(KeySeq col_seq) noexcept {
  switch(col_seq) {
    case KeySeq::FC_VOLUME:
      return 4;
    case KeySeq::FC_LEXIC:
      return 3;
    case KeySeq::VOLUME:
      return 2;
    default:
      return 1;
  }
}

cid_t get_meta_cid(KeySeq col_seq) noexcept {
  switch(col_seq) {
    case KeySeq::FC_VOLUME:
      return 8;
    case KeySeq::FC_LEXIC:
      return 7;
    case KeySeq::VOLUME:
      return 6;
    default:
      return 5;
  }
}

const char* get_meta_cid_str(KeySeq col_seq) noexcept {
  switch(col_seq) {
    case KeySeq::FC_VOLUME:
      return "8";
    case KeySeq::FC_LEXIC:
      return "7";
    case KeySeq::VOLUME:
      return "6";
    default:
      return "5";
  }
}

uint8_t get_sys_cid(KeySeq col_seq, Range col_type) noexcept {
  switch(col_seq) {
    case KeySeq::FC_VOLUME:
      return col_type == Range::DATA ? 8 : 4;
    case KeySeq::FC_LEXIC:
      return col_type == Range::DATA ? 7 : 3;
    case KeySeq::VOLUME:
      return col_type == Range::DATA ? 6 : 2;
    default:
      return col_type == Range::DATA ? 5 : 1;
  }
}

}}}}
