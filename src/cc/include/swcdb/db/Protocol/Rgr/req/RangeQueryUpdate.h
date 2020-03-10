
/*
 * Copyright (C) 2019 SWC-DB (author: Kashirin Alex (kashirin.alex@gmail.com))
 */

#ifndef swc_db_protocol_rgr_req_RangeQueryUpdate_h
#define swc_db_protocol_rgr_req_RangeQueryUpdate_h



#include "swcdb/core/comm/ClientConnQueue.h"
#include "swcdb/db/Protocol/Rgr/params/RangeQueryUpdate.h"


namespace SWC { namespace Protocol { namespace Rgr { namespace Req {

  
class RangeQueryUpdate: public client::ConnQueue::ReqBase {
  public:
  
  typedef std::function<void(client::ConnQueue::ReqBase::Ptr, 
                             const Params::RangeQueryUpdateRsp&)> Cb_t;

  static void 
  request(const Params::RangeQueryUpdateReq& params, DynamicBuffer::Ptr buffer,
          const EndPoints& endpoints, const Cb_t cb, 
          const uint32_t timeout = 10000);


  RangeQueryUpdate(const Params::RangeQueryUpdateReq& params,
                   DynamicBuffer::Ptr buffer, const EndPoints& endpoints,
                   const Cb_t cb, const uint32_t timeout);

  virtual ~RangeQueryUpdate();

  void handle_no_conn() override;

  bool run(uint32_t timeout=0) override;

  void handle(ConnHandlerPtr conn, Event::Ptr& ev) override;

  private:

  EndPoints     endpoints;
  const Cb_t    cb;
};


}}}}


#ifdef SWC_IMPL_SOURCE
#include "swcdb/db/Protocol/Rgr/req/RangeQueryUpdate.cc"
#endif 

#endif // swc_db_protocol_rgr_req_RangeQueryUpdate_h
