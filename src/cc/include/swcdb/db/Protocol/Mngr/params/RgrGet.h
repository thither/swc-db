
/*
 * Copyright (C) 2019 SWC-DB (author: Kashirin Alex (kashirin.alex@gmail.com))
 */

#ifndef swc_db_protocol_mngr_params_RgrGet_h
#define swc_db_protocol_mngr_params_RgrGet_h

#include "swcdb/core/Serializable.h"
#include "swcdb/db/Protocol/Common/params/HostEndPoints.h"

#include "swcdb/db/Cells/Cell.h"
#include "swcdb/db/Cells/SpecsInterval.h"


namespace SWC { namespace Protocol { namespace Mngr { namespace Params {


class RgrGetReq : public Serializable {
  public:

  RgrGetReq(int64_t cid=0, int64_t rid=0) : cid(cid), rid(rid) {}

  RgrGetReq(int64_t cid, 
            const DB::Cell::Key& range_begin, 
            const DB::Cell::Key& range_end)
            : cid(cid), rid(0), 
              range_begin(range_begin), range_end(range_end) {}

  virtual ~RgrGetReq(){ }
  
  int64_t        cid;
  int64_t        rid;
  DB::Cell::Key  range_begin, range_end;
  
  const std::string to_string() {
    std::string s("Ranger(");
    s.append("cid=");
    s.append(std::to_string(cid));
    s.append(" rid=");
    s.append(std::to_string(rid));
    if(!rid) {
      s.append(" RangeBegin");
      s.append(range_begin.to_string());
      s.append(" RangeEnd");
      s.append(range_end.to_string());
    }
    s.append(")");
    return s;
  }

  private:

  uint8_t encoding_version() const  {
    return 1; 
  }

  size_t encoded_length_internal() const {
    return Serialization::encoded_length_vi64(cid)
      + Serialization::encoded_length_vi64(rid)
      + (!rid ? range_begin.encoded_length() + range_end.encoded_length() : 0);
  }
    
  void encode_internal(uint8_t **bufp) const {
    Serialization::encode_vi64(bufp, cid);
    Serialization::encode_vi64(bufp, rid);
    if(!rid) {
      range_begin.encode(bufp);
      range_end.encode(bufp);
    }
  }
    
  void decode_internal(uint8_t version, const uint8_t **bufp, 
                       size_t *remainp) {
    cid = Serialization::decode_vi64(bufp, remainp);
    rid = Serialization::decode_vi64(bufp, remainp);
    if(!rid) {
      range_begin.decode(bufp, remainp);
      range_end.decode(bufp, remainp);
    }
  }

};



class RgrGetRsp : public Common::Params::HostEndPoints {
  public:

  RgrGetRsp(): err(0), cid(0), rid(0), next_range(false) {}

  RgrGetRsp(int64_t cid, int64_t rid, const EndPoints& endpoints) 
            : Common::Params::HostEndPoints(endpoints), 
              err(0), cid(cid), rid(rid), next_range(false) { }

  RgrGetRsp(int64_t cid, int64_t rid, const EndPoints& endpoints, 
            const DB::Cell::Key& range_end, 
            bool next_range)  
            : Common::Params::HostEndPoints(endpoints), 
              err(0), cid(cid), rid(rid), 
              range_end(range_end), next_range(next_range) { }

  int             err;         
  int64_t         cid; 
  int64_t         rid; 
  DB::Cell::Key   range_end;
  bool            next_range;

  const std::string to_string() const {
    std::string s("Ranger(");
    s.append("err=");
    s.append(std::to_string(err));
    if(err == Error::OK) {
      s.append(" cid=");
      s.append(std::to_string(cid));
      s.append(" rid=");
      s.append(std::to_string(rid));
      s.append(" ");
      s.append(Common::Params::HostEndPoints::to_string());
      if(cid == 1) {
        s.append(" RangeEnd");
        s.append(range_end.to_string());
        s.append(" next_range=");
        s.append(std::to_string(next_range));
      }
    } else {
      s.append("(");
      s.append(Error::get_text(err));
      s.append(")");
    }
    s.append(")");
    return s;
  }

  private:

  uint8_t encoding_version() const {
    return 1;
  }
    
  size_t encoded_length_internal() const {
    return  Serialization::encoded_length_vi32(err) 
    + (err != Error::OK ? 0 :
        (Serialization::encoded_length_vi64(cid)
        + Serialization::encoded_length_vi64(rid)
        + Common::Params::HostEndPoints::encoded_length_internal()
        + (cid==1 ? range_end.encoded_length()+1 : 0)
        )
      );
  }
    
  void encode_internal(uint8_t **bufp) const {
    Serialization::encode_vi32(bufp, err);
    if(err == Error::OK) {
      Serialization::encode_vi64(bufp, cid);
      Serialization::encode_vi64(bufp, rid);
      Common::Params::HostEndPoints::encode_internal(bufp);
      if(cid == 1) {
        range_end.encode(bufp);
        Serialization::encode_bool(bufp, next_range);
      }
    }
  }
    
  void decode_internal(uint8_t version, const uint8_t **bufp, 
                       size_t *remainp) {
    err = Serialization::decode_vi32(bufp, remainp);
    if(err == Error::OK) {
      cid = Serialization::decode_vi64(bufp, remainp);
      rid = Serialization::decode_vi64(bufp, remainp);
      Common::Params::HostEndPoints::decode_internal(version, bufp, remainp);
      if(cid == 1) {
        range_end.decode(bufp, remainp, true);
        next_range = Serialization::decode_bool(bufp, remainp);
      }
    }
  }

};
  

}}}}

#endif // swc_db_protocol_mngr_params_RgrGet_h
