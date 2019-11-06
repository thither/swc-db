/*
 * Copyright (C) 2019 SWC-DB (author: Kashirin Alex (kashirin.alex@gmail.com))
 */

#ifndef swc_lib_fs_Broker_Protocol_req_Mkdirs_h
#define swc_lib_fs_Broker_Protocol_req_Mkdirs_h

#include "Base.h"
#include "../params/Mkdirs.h"

namespace SWC { namespace FS { namespace Protocol { namespace Req {

class Mkdirs : public Base {

  public:

  Mkdirs(uint32_t timeout, const String &name, Callback::MkdirsCb_t cb=0) 
        : name(name), cb(cb) {
    HT_DEBUGF("mkdirs path='%s'", name.c_str());

    cbp = CommBuf::make(Params::MkdirsReq(name));
    cbp->header.set(Cmd::FUNCTION_MKDIRS, timeout);
  }

  std::promise<void> promise(){
    std::promise<void>  r_promise;
    cb = [await=&r_promise](int err){await->set_value();};
    return r_promise;
  }

  void handle(ConnHandlerPtr conn, Event::Ptr &ev) { 

    const uint8_t *ptr;
    size_t remain;

    if(!Base::is_rsp(conn, ev, Cmd::FUNCTION_MKDIRS, &ptr, &remain))
      return;

    HT_DEBUGF("mkdirs path='%s' error='%d'", name.c_str(), error);
    
    cb(error);
  }

  private:
  const String          name;
  Callback::MkdirsCb_t  cb;
};
typedef std::shared_ptr<Mkdirs> MkdirsPtr;



}}}}

#endif  // swc_lib_fs_Broker_Protocol_req_Mkdirs_h