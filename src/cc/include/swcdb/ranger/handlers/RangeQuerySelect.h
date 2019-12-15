/*
 * Copyright (C) 2019 SWC-DB (author: Kashirin Alex (kashirin.alex@gmail.com))
 */


#ifndef swc_app_ranger_handlers_RangeQuerySelect_h
#define swc_app_ranger_handlers_RangeQuerySelect_h

#include "swcdb/db/Protocol/Rgr/params/RangeQuerySelect.h"
#include "swcdb/ranger/callbacks/RangeQuerySelect.h"


namespace SWC { namespace Protocol { namespace Rgr { namespace Handler {


void range_query_select(ConnHandlerPtr conn, Event::Ptr ev) {
  int err = Error::OK;
  Params::RangeQuerySelectReq params;
  server::Rgr::Range::Ptr range;

  try {
    const uint8_t *ptr = ev->data.base;
    size_t remain = ev->data.size;
    params.decode(&ptr, &remain);

    range =  Env::RgrColumns::get()->get_range(err, params.cid, params.rid);
 
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
      params.interval.flags.max_versions ? 
      params.interval.flags.max_versions : (uint32_t)range->cfg->cell_versions,
      range->cfg->cell_ttl, 
      range->cfg->col_type
    );
    range->scan(
      std::make_shared<server::Rgr::Callback::RangeQuerySelect>(
        conn, ev, params.interval, cells, range, params.limit_buffer_sz
      )
    );
  } catch (Exception &e) {
    SWC_LOG_OUT(LOG_ERROR) << e << SWC_LOG_OUT_END;
  }
  
}
  

}}}}

#endif // swc_app_ranger_handlers_RangeQuerySelect_h