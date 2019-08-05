
/*
 * Copyright (C) 2019 SWC-DB (author: Kashirin Alex (kashirin.alex@gmail.com))
 */

#ifndef swc_lib_manager_RangeServerStatus_h
#define swc_lib_manager_RangeServerStatus_h

#include "swcdb/lib/db/Protocol/params/HostEndPoints.h"


namespace SWC { namespace server { namespace Mngr {

class RangeServerStatus : public Protocol::Params::HostEndPoints {

  public:

  enum State {
    NONE,
    ACK,
    REMOVED
  };
  
  RangeServerStatus(): rs_id(0), state(State::NONE), 
                       failures(0), total_ranges(0) {}
                       
  RangeServerStatus(uint64_t rs_id, EndPoints endpoints)
                    : rs_id(rs_id), state(State::NONE), 
                      failures(0), total_ranges(0),
                      Protocol::Params::HostEndPoints(endpoints) {}

  virtual ~RangeServerStatus(){}

  std::string to_string(){
    std::string s("[rs_id=");
    s.append(std::to_string(rs_id));
    s.append(", state=");
    s.append(std::to_string(state));
    s.append(", failures=");
    s.append(std::to_string(failures));
    s.append(", total_ranges=");
    s.append(std::to_string(total_ranges));
    s.append(", ");
    s.append(Protocol::Params::HostEndPoints::to_string());
    s.append("]");
    return s;
  }

  size_t encoded_length_internal() const {
    size_t len = 1 +  
               + Serialization::encoded_length_vi64(rs_id)
               + Protocol::Params::HostEndPoints::encoded_length_internal();
    return len;
  }

  void encode_internal(uint8_t **bufp) const {
    Serialization::encode_i8(bufp, (uint8_t)state);
    Serialization::encode_vi64(bufp, rs_id);
    Protocol::Params::HostEndPoints::encode_internal(bufp);
  }

  void decode_internal(uint8_t version, const uint8_t **bufp, size_t *remainp){
    state = (State)Serialization::decode_i8(bufp, remainp);
    rs_id = Serialization::decode_vi64(bufp, remainp);

    Protocol::Params::HostEndPoints::decode_internal(version, bufp, remainp);
  }

  uint64_t   rs_id;
  State      state;

  int32_t    failures;
  size_t     total_ranges;
};
typedef std::shared_ptr<RangeServerStatus> RangeServerStatusPtr;
typedef std::vector<RangeServerStatusPtr>  RangeServerStatusList;

}}}

#endif // swc_lib_manager_RangeServerStatus_h