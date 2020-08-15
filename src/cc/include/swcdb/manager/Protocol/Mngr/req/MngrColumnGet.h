
/*
 * Copyright Since 2019 SWC-DB© [author: Kashirin Alex kashirin.alex@gmail.com]
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */ 

#ifndef swc_manager_Protocol_mngr_req_MngrColumnGet_h
#define swc_manager_Protocol_mngr_req_MngrColumnGet_h

#include "swcdb/db/Protocol/Mngr/params/ColumnGet.h"

namespace SWC { namespace Protocol { namespace Mngr { namespace Req {

class MngrColumnGet : public client::ConnQueue::ReqBase {
  public:
  
  typedef std::function<void(int, const Params::ColumnGetRsp&)> Cb_t;

  MngrColumnGet(const Params::ColumnGetReq& params, const Cb_t& cb) 
                : client::ConnQueue::ReqBase(true), cb(cb) {
    cbp = CommBuf::make(params);
    cbp->header.set(COLUMN_GET, 60000);
  }
  
  virtual ~MngrColumnGet() { }
  
  void handle(ConnHandlerPtr, const Event::Ptr& ev) override {
    if(!is_rsp(ev))
      return;

    Params::ColumnGetRsp rsp_params;
    int err = ev->response_code();
    if(!err) {
      try {
        const uint8_t *ptr = ev->data.base + 4;
        size_t remain = ev->data.size - 4;
        rsp_params.decode(&ptr, &remain);
      } catch (Exception &e) {
        SWC_LOG_OUT(LOG_ERROR) << e << SWC_LOG_OUT_END;
        err = e.code();
      }
    }

    cb(err, rsp_params);
  }

  private:
  Cb_t   cb;
  
};

}}}}

#endif // swc_manager_Protocol_mngr_req_MngrColumnGet_h
