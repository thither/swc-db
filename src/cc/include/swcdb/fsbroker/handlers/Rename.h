/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#ifndef swc_fsbroker_handlers_Rename_h
#define swc_fsbroker_handlers_Rename_h

#include "swcdb/fs/Broker/Protocol/params/Rename.h"


namespace SWC { namespace FsBroker { namespace Handler {


void rename(const ConnHandlerPtr& conn, const Event::Ptr& ev) {

  int err = Error::OK;
  try {

    const uint8_t *ptr = ev->data.base;
    size_t remain = ev->data.size;

    FS::Protocol::Params::RenameReq params;
    params.decode(&ptr, &remain);

    Env::FsInterface::fs()->rename(err, params.from, params.to);

  } catch(...) {
    const Exception& e = SWC_CURRENT_EXCEPTION("");
    SWC_LOG_OUT(LOG_ERROR, SWC_LOG_OSTREAM << e; );
    err = e.code();
  }
  
  if(ev->expired())
    return;

  try {
    auto cbp = CommBuf::make(4);
    cbp->header.initialize_from_request_header(ev->header);
    cbp->append_i32(err);
    conn->send_response(cbp);

  } catch(...) {
    SWC_LOG_CURRENT_EXCEPTION("");
  }

}
  

}}}

#endif // swc_fsbroker_handlers_Rename_h