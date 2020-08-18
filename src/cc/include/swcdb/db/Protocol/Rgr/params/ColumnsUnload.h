
/*
 * Copyright Since 2019 SWC-DB© [author: Kashirin Alex kashirin.alex@gmail.com]
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swc_db_protocol_rgr_params_ColumnsUnload_h
#define swc_db_protocol_rgr_params_ColumnsUnload_h

#include "swcdb/core/Serializable.h"


namespace SWC { namespace Protocol { namespace Rgr { namespace Params {

class ColumnsUnloadRsp : public Serializable {
  public:

  ColumnsUnloadRsp(int err = Error::OK) : err(err) { }
  
             
  virtual ~ColumnsUnloadRsp() {}

  int err;
  std::unordered_map<cid_t, std::vector<rid_t>> columns;
  
  private:

  size_t internal_encoded_length() const {
    size_t sz = Serialization::encoded_length_vi32(err);
    if(err)
      return sz;
    sz += Serialization::encoded_length_vi64(columns.size());
    for(auto it = columns.begin(); it != columns.end(); ++it) {
      sz += Serialization::encoded_length_vi64(it->first);
      sz += Serialization::encoded_length_vi64(it->second.size());
      for(auto& r : it->second)
        sz += Serialization::encoded_length_vi64(r);
    }
    return sz;
  }
    
  void internal_encode(uint8_t** bufp) const {
    Serialization::encode_vi32(bufp, err);
    if(err)
      return;
    Serialization::encode_vi64(bufp, columns.size());
    for(auto it = columns.begin(); it != columns.end(); ++it) {
      Serialization::encode_vi64(bufp, it->first);
      Serialization::encode_vi64(bufp, it->second.size());
      for(auto& r : it->second)
        Serialization::encode_vi64(bufp, r);
    }
  }
    
  void internal_decode(const uint8_t** bufp, size_t* remainp) {
    err = Serialization::decode_vi32(bufp, remainp);
    if(err)
      return;
    cid_t cid;
    for(size_t cids=Serialization::decode_vi64(bufp, remainp); cids; --cids) {
      cid = Serialization::decode_vi64(bufp, remainp);
      std::vector<rid_t> rids;
      rids.resize(Serialization::decode_vi64(bufp, remainp));
      for(auto it = rids.begin(); it < rids.end(); ++it)
        *it = Serialization::decode_vi64(bufp, remainp);
      columns.emplace(cid, rids);
    }
  }

};
  

}}}}

#endif // swc_db_protocol_rgr_params_ColumnsUnload_h