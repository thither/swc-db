/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swcdb_db_protocol_rgr_req_RangeQuerySelect_h
#define swcdb_db_protocol_rgr_req_RangeQuerySelect_h


#include "swcdb/core/comm/ClientConnQueue.h"
#include "swcdb/db/Protocol/Rgr/params/RangeQuerySelect.h"


namespace SWC { namespace Comm { namespace Protocol {
namespace Rgr { namespace Req {


class RangeQuerySelect: public client::ConnQueue::ReqBase {
  public:

  typedef std::function<void(const client::ConnQueue::ReqBase::Ptr&,
                             Params::RangeQuerySelectRsp&)> Cb_t;

  static void
  request(const SWC::client::Clients::Ptr& clients,
          const Params::RangeQuerySelectReq& params,
          const EndPoints& endpoints,
          Cb_t&& cb,
          const uint32_t timeout = 10000);

  EndPoints     endpoints;

  RangeQuerySelect(const SWC::client::Clients::Ptr& clients,
                   const Params::RangeQuerySelectReq& params,
                   const EndPoints& endpoints,
                   Cb_t&& cb,
                   const uint32_t timeout);

  virtual ~RangeQuerySelect() { }

  void handle_no_conn() override;

  bool run() override;

  void handle(ConnHandlerPtr conn, const Event::Ptr& ev) override;

  private:
  SWC::client::Clients::Ptr clients;
  const Cb_t                cb;
};


}}}}}


#ifdef SWC_IMPL_SOURCE
#include "swcdb/db/Protocol/Rgr/req/RangeQuerySelect.cc"
#endif

#endif // swcdb_db_protocol_rgr_req_RangeQuerySelect_h
