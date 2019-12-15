/*
 * Copyright (C) 2019 SWC-DB (author: Kashirin Alex (kashirin.alex@gmail.com))
 */


#ifndef swc_app_ranger_handlers_RangeLocate_h
#define swc_app_ranger_handlers_RangeLocate_h

#include "swcdb/db/Protocol/Rgr/params/RangeLocate.h"
#include "swcdb/ranger/callbacks/RangeLocateScan.h"


namespace SWC { namespace Protocol { namespace Rgr { namespace Handler {


void range_locate(ConnHandlerPtr conn, Event::Ptr ev) {
  int err = Error::OK;
  Params::RangeLocateReq params;
  server::Rgr::Range::Ptr range;

  try {
    const uint8_t *ptr = ev->data.base;
    size_t remain = ev->data.size;
    params.decode(&ptr, &remain);

    range = Env::RgrColumns::get()->get_range(err, params.cid, params.rid);
    
    if(!err && (range == nullptr || !range->is_loaded()))
      err = Error::RS_NOT_LOADED_RANGE;

  } catch (Exception &e) {
    SWC_LOG_OUT(LOG_ERROR) << e << SWC_LOG_OUT_END;
    err = e.code();
  }

  try{

    if(err) {
      conn->send_error(err, "", ev);
      return;
    }

    DB::Cells::Mutable cells(
      params.interval.flags.limit, 
      range->cfg->cell_versions, 
      range->cfg->cell_ttl, 
      range->cfg->col_type
    );
    auto req = std::make_shared<server::Rgr::Callback::RangeLocateScan>(
      conn, ev, params.interval, cells, range
    );
    req->spec.key_finish.copy(req->spec.key_start);
    req->spec.key_finish.set(-1, Condition::LE);
    if(range->type != Types::Range::DATA) 
      req->spec.key_finish.set(0, Condition::EQ);
    req->spec.flags.limit = 2;

    range->scan(req);
    
  } catch (Exception &e) {
    SWC_LOG_OUT(LOG_ERROR) << e << SWC_LOG_OUT_END;
  }
  
}
  

}}}}

#endif // swc_app_ranger_handlers_RangeLocate_h