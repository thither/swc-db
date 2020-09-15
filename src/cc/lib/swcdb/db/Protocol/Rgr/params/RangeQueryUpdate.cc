
/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */
 


#include "swcdb/db/Protocol/Rgr/params/RangeQueryUpdate.h"
#include "swcdb/core/Serialization.h"
#include "swcdb/core/Error.h"


namespace SWC { namespace Protocol { namespace Rgr { namespace Params {

RangeQueryUpdateReq::RangeQueryUpdateReq() {}

RangeQueryUpdateReq::RangeQueryUpdateReq(cid_t cid, rid_t rid)
                                        : cid(cid), rid(rid) {}

RangeQueryUpdateReq::~RangeQueryUpdateReq() { }

void RangeQueryUpdateReq::print(std::ostream& out) const {
  out << "RangeQueryUpdateReq(cid=" << cid << " rid=" << rid << ')';
}

size_t RangeQueryUpdateReq::internal_encoded_length() const {
  return  Serialization::encoded_length_vi64(cid)
        + Serialization::encoded_length_vi64(rid);
}
  
void RangeQueryUpdateReq::internal_encode(uint8_t** bufp) const {
  Serialization::encode_vi64(bufp, cid);
  Serialization::encode_vi64(bufp, rid);
}
  
void RangeQueryUpdateReq::internal_decode(const uint8_t** bufp, 
                                          size_t* remainp) {
  cid = Serialization::decode_vi64(bufp, remainp);
  rid = Serialization::decode_vi64(bufp, remainp);
}




RangeQueryUpdateRsp::RangeQueryUpdateRsp(int err) : err(err) {  }

RangeQueryUpdateRsp::RangeQueryUpdateRsp(int err, 
                      const DB::Cell::Key& range_prev_end, 
                      const DB::Cell::Key& range_end) 
                    : err(err), 
                      range_prev_end(range_prev_end), range_end(range_end) {
}

RangeQueryUpdateRsp::~RangeQueryUpdateRsp() { }

void RangeQueryUpdateRsp::print(std::ostream& out) const {
  Error::print(out << "RangeQueryUpdateRsp(", err);
  if(err == Error::RANGE_BAD_INTERVAL) {
    if(!range_prev_end.empty())
      range_prev_end.print(out << " range_prev_end=");
    if(!range_end.empty())
      range_end.print(out << " range_end=");
  }
  out << ')';
}

size_t RangeQueryUpdateRsp::internal_encoded_length() const {
  return  Serialization::encoded_length_vi32(err)
        + (err == Error::RANGE_BAD_INTERVAL 
          ? range_prev_end.encoded_length() + range_end.encoded_length() 
          : 0);
}
  
void RangeQueryUpdateRsp::internal_encode(uint8_t** bufp) const {
  Serialization::encode_vi32(bufp, err);
  if(err == Error::RANGE_BAD_INTERVAL) {
    range_prev_end.encode(bufp);
    range_end.encode(bufp);
  }
}
  
void RangeQueryUpdateRsp::internal_decode(const uint8_t** bufp, 
                                          size_t* remainp) {
  err = Serialization::decode_vi32(bufp, remainp);
  if(err == Error::RANGE_BAD_INTERVAL) {
    range_prev_end.decode(bufp, remainp, true);
    range_end.decode(bufp, remainp, true);
  }
}



}}}}
