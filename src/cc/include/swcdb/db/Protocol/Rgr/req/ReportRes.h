
/*
 * Copyright Since 2019 SWC-DB© [author: Kashirin Alex kashirin.alex@gmail.com]
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swc_db_protocol_rgr_req_ReportRes_h
#define swc_db_protocol_rgr_req_ReportRes_h


#include "swcdb/db/Protocol/Rgr/params/Report.h"


namespace SWC { namespace Protocol { namespace Rgr { namespace Req {

  
class ReportRes: public client::ConnQueue::ReqBase {
  public:
  
  typedef std::function<void(const client::ConnQueue::ReqBase::Ptr&, 
                             const Params::ReportResRsp&)> Cb_t;
  typedef std::function<void()> Cb_no_conn_t;

  static void request(const Params::ReportReq& params, 
                      const EndPoints& endpoints,
                      const Cb_no_conn_t& cb_no_conn, const Cb_t& cb, 
                      const uint32_t timeout = 10000);

  ReportRes(const Params::ReportReq& params, 
            const EndPoints& endpoints, 
            const Cb_no_conn_t& cb_no_conn, const Cb_t& cb, 
            const uint32_t timeout);

  virtual ~ReportRes();

  void handle_no_conn() override;

  bool run() override;

  void handle(ConnHandlerPtr conn, const Event::Ptr& ev) override;

  private:

  EndPoints     endpoints;
  Cb_no_conn_t  cb_no_conn;
  const Cb_t    cb;
};


}}}}


#ifdef SWC_IMPL_SOURCE
#include "swcdb/db/Protocol/Rgr/req/Report.cc"
#endif 

#endif // swc_db_protocol_rgr_req_Report_h