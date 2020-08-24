/*
 * Copyright Since 2019 SWC-DB© [author: Kashirin Alex kashirin.alex@gmail.com]
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#ifndef swc_ranger_Protocol_handlers_ColumnUpdate_h
#define swc_ranger_Protocol_handlers_ColumnUpdate_h

#include "swcdb/db/Protocol/Rgr/params/ColumnUpdate.h"


namespace SWC { namespace Protocol { namespace Rgr { namespace Handler {


void column_update(const ConnHandlerPtr& conn, const Event::Ptr& ev) {
  try {
    const uint8_t *ptr = ev->data.base;
    size_t remain = ev->data.size;

    Params::ColumnUpdate params;
    params.decode(&ptr, &remain);
    
    int err = Error::OK;
    auto col = RangerEnv::columns()->get_column(err, params.schema->cid);
    if(col != nullptr) {
      col->schema_update(*params.schema.get());

      if(!RangerEnv::is_shuttingdown())
        SWC_LOGF(LOG_DEBUG, "updated %s", col->cfg.to_string().c_str());
    }
    conn->response_ok(ev);

  } catch(...) {
    SWC_LOG_CURRENT_EXCEPTION("");
  }
  
}
  

}}}}

#endif // swc_ranger_Protocol_handlers_ColumnUpdate_h