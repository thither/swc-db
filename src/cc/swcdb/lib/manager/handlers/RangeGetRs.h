/*
 * Copyright (C) 2019 SWC-DB (author: Kashirin Alex (kashirin.alex@gmail.com))
 */


#ifndef swc_app_manager_handlers_RangeGetRs_h
#define swc_app_manager_handlers_RangeGetRs_h

#include "swcdb/lib/db/Protocol/params/MngrRangeGetRs.h"


namespace SWC { namespace server { namespace Mngr {

namespace Handler {


class RangeGetRs : public AppHandler {
  public:

  RangeGetRs(ConnHandlerPtr conn, EventPtr ev)
            : AppHandler(conn, ev){ }

  void run() override {

    int err = Error::OK;
    
    try {

      const uint8_t *ptr = m_ev->payload;
      size_t remain = m_ev->payload_len;

      Protocol::Params::MngrRangeGetRsReq params;
      params.decode(&ptr, &remain);

      if(!Env::MngrRole::get()->is_active(params.cid)){
        std::cout << "MNGR NOT ACTIVE: cid=" << params.cid << "\n";
        err = Error::MNGR_NOT_ACTIVE;
        goto send_error;
      }
      auto col = Env::MngrColumns::get()->get_column(err, params.cid, false);
      if(err != Error::OK)
        goto send_error;

      col->state(err);
      if(err != Error::OK)
        goto send_error;
      
      bool next = false;
      server::Mngr::RangePtr range;
      if(params.cid == 1){
        range = col->get_range(err, params.intervals, next);
      } else {
        range = col->get_range(err, params.rid);
      }
      if(range == nullptr) {
        err = Error::RANGE_NOT_FOUND;
        goto send_error;
      }

      EndPoints endpoints;
      Env::RangeServers::get()->rs_get(range->get_rs_id(), endpoints);
      if(endpoints.empty()) {
        err = Error::RS_NOT_READY;
        goto send_error;
      }

      Protocol::Params::MngrRangeGetRsRsp rsp_params(
        range->cid,
        range->rid, 
        endpoints, 
        next
      );
      
      CommHeader header;
      header.initialize_from_request_header(m_ev->header);
      CommBufPtr cbp = std::make_shared<CommBuf>(
        header, rsp_params.encoded_length()+4);
      cbp->append_i32(err);
      rsp_params.encode(cbp->get_data_ptr_address());
      m_conn->send_response(cbp);
      return;
    }
    catch (Exception &e) {
      HT_ERROR_OUT << e << HT_END;
      err = e.code();
    }
  
    send_error:
      try {
        CommHeader header;
        header.initialize_from_request_header(m_ev->header);
        CommBufPtr cbp = std::make_shared<CommBuf>(header, 4);
        cbp->append_i32(err);
        m_conn->send_response(cbp);
      } catch (Exception &e) {
        HT_ERROR_OUT << e << HT_END;
      }
    
  }

};
  
// flag(if cid==1) 
//      in(cid+intervals)  out(cid + rid + rs-endpoints + ?next) 
// else 
//      in(cid+rid)        out(cid + rid + rs-endpoints)

}}}}

#endif // swc_app_manager_handlers_RangeGetRs_h