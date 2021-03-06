/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#ifndef swcdb_ranger_Protocol_handlers_AssignId_h
#define swcdb_ranger_Protocol_handlers_AssignId_h


namespace SWC { namespace Comm { namespace Protocol {
namespace Rgr { namespace Handler {


void assign_id(const ConnHandlerPtr& conn, const Event::Ptr& ev, 
               Mngr::Req::RgrMngId::Ptr id_mngr) {

  Env::Rgr::is_not_accepting() 
    ? conn->send_error(Error::SERVER_SHUTTING_DOWN, "", ev)
    : conn->response_ok(ev);

  id_mngr->request();

}


}}}}}

#endif // swcdb_ranger_Protocol_handlers_AssignId_h
