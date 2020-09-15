/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#ifndef swc_ranger_Protocol_handlers_AssignId_h
#define swc_ranger_Protocol_handlers_AssignId_h


namespace SWC { namespace Protocol { namespace Rgr { namespace Handler {


void assign_id(const ConnHandlerPtr& conn, const Event::Ptr& ev, 
               Protocol::Mngr::Req::RgrMngId::Ptr id_mngr) {
  try {

    if(RangerEnv::is_shuttingdown())
      conn->send_error(Error::SERVER_SHUTTING_DOWN, "", ev);
    else 
      conn->response_ok(ev);

    id_mngr->request();

  } catch(...) {
    SWC_LOG_CURRENT_EXCEPTION("");
  }
}


}}}}

#endif // swc_ranger_Protocol_handlers_AssignId_h