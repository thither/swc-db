
/*
 * Copyright Since 2019 SWC-DB© [author: Kashirin Alex kashirin.alex@gmail.com]
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swc_db_protocol_mngr_req_ColumnList_h
#define swc_db_protocol_mngr_req_ColumnList_h


#include "swcdb/db/Protocol/Mngr/params/ColumnList.h"
#include "swcdb/core/comm/ClientConnQueue.h"


namespace SWC { namespace Protocol { namespace Mngr { namespace Req {

  
class ColumnList: public client::ConnQueue::ReqBase {
  public:
  
  typedef std::function<void(const client::ConnQueue::ReqBase::Ptr&, 
                             int, const Params::ColumnListRsp&)> Cb_t;

  static void request(const Cb_t& cb, const uint32_t timeout = 10000);

  static void request(const Params::ColumnListReq& params,
                      const ColumnList::Cb_t& cb, 
                      const uint32_t timeout = 10000);

  ColumnList(const Params::ColumnListReq& params, const Cb_t& cb, 
             const uint32_t timeout);

  virtual ~ColumnList();

  void handle_no_conn() override;

  bool run() override;

  void handle(ConnHandlerPtr conn, const Event::Ptr& ev) override;

  private:
  
  void clear_endpoints();

  const Cb_t  cb;
  EndPoints   endpoints;
};



}}}}


#ifdef SWC_IMPL_SOURCE
#include "swcdb/db/Protocol/Mngr/req/ColumnList.cc"
#endif 

#endif // swc_db_protocol_mngr_req_ColumnList_h
