/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#ifndef swc_manager_Protocol_handlers_MngrState_h
#define swc_manager_Protocol_handlers_MngrState_h


namespace SWC { namespace Protocol { namespace Mngr { namespace Handler {


void mngr_state(const Comm::ConnHandlerPtr& conn, const Comm::Event::Ptr& ev) {
  try {
    const uint8_t *ptr = ev->data.base;
    size_t remain = ev->data.size;

    Params::MngrState req_params;
    req_params.decode(&ptr, &remain);

    Env::Mngr::role()->fill_states(
      req_params.states, req_params.token, 
      nullptr // std::make_shared<Comm::ResponseCallback>(conn, ev)
    ); 

    Env::Mngr::role()->update_manager_addr(
      conn->endpoint_remote_hash(), req_params.mngr_host);

    conn->response_ok(ev);

  } catch(...) {
    SWC_LOG_CURRENT_EXCEPTION("");
  }
}
  

}}}}

#endif // swc_manager_Protocol_handlers_MngrState_h