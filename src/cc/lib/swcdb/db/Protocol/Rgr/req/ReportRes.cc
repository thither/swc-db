
/*
 * Copyright Since 2019 SWC-DB© [author: Kashirin Alex kashirin.alex@gmail.com]
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#include "swcdb/db/client/Clients.h"
#include "swcdb/db/Protocol/Commands.h"
#include "swcdb/db/Protocol/Rgr/req/ReportRes.h"


namespace SWC { namespace Protocol { namespace Rgr { namespace Req {



SWC_SHOULD_INLINE
void ReportRes::request(const Params::ReportReq& params, 
                     const EndPoints& endpoints, 
                     const ReportRes::Cb_no_conn_t& cb_no_conn, 
                     const ReportRes::Cb_t& cb, 
                     const uint32_t timeout) {
  std::make_shared<ReportRes>(params, endpoints, cb_no_conn, cb, timeout)->run();
}

ReportRes::ReportRes(const Params::ReportReq& params, 
               const EndPoints& endpoints, 
               const ReportRes::Cb_no_conn_t& cb_no_conn, 
               const ReportRes::Cb_t& cb, 
               const uint32_t timeout) 
              : client::ConnQueue::ReqBase(false), 
                endpoints(endpoints), cb_no_conn(cb_no_conn), cb(cb) {
  cbp = CommBuf::make(params);
  cbp->header.set(REPORT, timeout);
}

ReportRes::~ReportRes() { }

void ReportRes::handle_no_conn() {
  cb_no_conn();
}

bool ReportRes::run() {
  Env::Clients::get()->rgr->get(endpoints)->put(req());
  return true;
}

void ReportRes::handle(ConnHandlerPtr, const Event::Ptr& ev) {

  if(ev->type == Event::Type::DISCONNECT){
    handle_no_conn();
    return;
  }

  Params::ReportResRsp rsp_params;
  if(ev->type == Event::Type::ERROR){
    rsp_params.err = ev->error;
    cb(req(), rsp_params);
    return;
  }

  try{
    const uint8_t *ptr = ev->data.base;
    size_t remain = ev->data.size;
    rsp_params.decode(&ptr, &remain);
  } catch (Exception &e) {
    SWC_LOG_OUT(LOG_ERROR) << e << SWC_LOG_OUT_END;
    rsp_params.err = e.code();
  }
  cb(req(), rsp_params);
}



}}}}