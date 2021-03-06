/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#include "swcdb/db/client/Query/Update/Committer.h"
#include "swcdb/db/Types/SystemColumn.h"
#include "swcdb/db/client/Clients.h"
#include "swcdb/db/Protocol/Rgr/req/RangeQueryUpdate.h"


namespace SWC { namespace client { namespace Query { namespace Update {



#define SWC_LOCATOR_REQ_DEBUG(msg) \
  SWC_LOG_OUT(LOG_DEBUG, params.print(SWC_LOG_OSTREAM << msg << ' '); );

#define SWC_LOCATOR_RSP_DEBUG(msg) \
  SWC_LOG_OUT(LOG_DEBUG, \
    print(SWC_LOG_OSTREAM << msg << ' '); \
    rsp.print(SWC_LOG_OSTREAM << ' '); \
  );

#define SWC_UPDATER_LOCATOR_RSP_DEBUG(msg) \
  SWC_LOG_OUT(LOG_DEBUG, \
    committer->print(SWC_LOG_OSTREAM << msg << ' '); \
    rsp.print(SWC_LOG_OSTREAM << ' '); \
  );



Committer::Committer(const DB::Types::Range type,
                     const cid_t cid,
                     Handlers::Base::Column* colp,
                     const DB::Cell::Key::Ptr& key_start,
                     const Handlers::Base::Ptr& hdlr,
                     const ReqBase::Ptr& parent,
                     const rid_t rid) noexcept
              : type(type), cid(cid), colp(colp), key_start(key_start),
                hdlr(hdlr), parent(parent), rid(rid) {
}

Committer::Committer(const DB::Types::Range type,
                     const cid_t cid,
                     Handlers::Base::Column* colp,
                     const DB::Cell::Key::Ptr& key_start,
                     const Handlers::Base::Ptr& hdlr,
                     const ReqBase::Ptr& parent,
                     const rid_t rid,
                     const DB::Cell::Key& key_finish)
              : type(type), cid(cid), colp(colp), key_start(key_start),
                hdlr(hdlr), parent(parent), rid(rid), key_finish(key_finish) {
}

void Committer::print(std::ostream& out) {
  out << "Committer(type=" << DB::Types::to_string(type)
      << " cid=" << cid << " rid=" << rid
      << " completion=" << hdlr->completion.count();
  key_start->print(out << " Start");
  key_finish.print(out << " Finish");
  colp->print(out << ' ');
  out << ')';
}

void Committer::locate_on_manager() {
  hdlr->completion.increment();
  if(!hdlr->valid())
    return hdlr->response();

  Comm::Protocol::Mngr::Params::RgrGetReq params(
    DB::Types::SystemColumn::get_master_cid(colp->get_sequence()));
  params.range_begin.copy(*key_start.get());
  if(DB::Types::SystemColumn::is_data(cid))
    params.range_begin.insert(0, std::to_string(cid));
  if(!DB::Types::SystemColumn::is_master(cid))
    params.range_begin.insert(
      0, DB::Types::SystemColumn::get_meta_cid_str(colp->get_sequence()));

  SWC_LOCATOR_REQ_DEBUG("mngr_locate_master");

  Comm::Protocol::Mngr::Req::RgrGet::request(
    hdlr->clients,
    params,
    [profile=hdlr->profile.mngr_locate(), committer=shared_from_this()]
    (const ReqBase::Ptr& req,
     const Comm::Protocol::Mngr::Params::RgrGetRsp& rsp) {
      profile.add(rsp.err || !rsp.rid);
      committer->located_on_manager(req, rsp);
    }
  );
}


void Committer::located_on_manager(
      const ReqBase::Ptr& base,
      const Comm::Protocol::Mngr::Params::RgrGetRsp& rsp) {
  if(!hdlr->valid())
    return hdlr->response();
  switch(rsp.err) {
    case Error::OK:
      break;
    case Error::COLUMN_NOT_EXISTS: {
      SWC_LOCATOR_RSP_DEBUG("mngr_located_master");
      if(cid == colp->get_cid())
        hdlr->error(cid, rsp.err);
      return hdlr->response(rsp.err);
    }
    case Error::CLIENT_STOPPING: {
      SWC_LOCATOR_RSP_DEBUG("mngr_located_master STOPPED");
      hdlr->error(rsp.err);
      return hdlr->response(rsp.err);
    }
    default: {
      SWC_LOCATOR_RSP_DEBUG("mngr_located_master RETRYING");
      return base->request_again();
    }
  }
  if(!rsp.rid) {
    SWC_LOCATOR_RSP_DEBUG("mngr_located_master RETRYING(no-rid)");
    return base->request_again();
  }

  SWC_LOCATOR_RSP_DEBUG("mngr_located_master");

  auto committer = std::make_shared<Committer>(
    DB::Types::Range::MASTER,
    rsp.cid, colp, key_start,
    hdlr, base,
    rsp.rid
  );
  DB::Types::SystemColumn::is_master(colp->get_cid())
    ? committer->commit_data(rsp.endpoints, base)
    : committer->locate_on_ranger(rsp.endpoints);

  if(!rsp.range_end.empty()) {
    auto next_key_start = colp->get_key_next(rsp.range_end);
    if(next_key_start) {
      std::make_shared<Committer>(
        DB::Types::Range::MASTER,
        colp->get_cid(), colp, next_key_start,
        hdlr
      )->locate_on_manager();
    }
  }
  hdlr->response();
}

void Committer::locate_on_ranger(const Comm::EndPoints& endpoints) {
  hdlr->completion.increment();
  if(!hdlr->valid())
    return hdlr->response();

  Comm::Protocol::Rgr::Params::RangeLocateReq params(cid, rid);
  params.flags |= Comm::Protocol::Rgr::Params::RangeLocateReq::COMMIT;

  params.range_begin.copy(*key_start.get());
  if(!DB::Types::SystemColumn::is_master(colp->get_cid())) {
    params.range_begin.insert(0, std::to_string(colp->get_cid()));
    if(type == DB::Types::Range::MASTER &&
       DB::Types::SystemColumn::is_data(colp->get_cid()))
      params.range_begin.insert(
        0, DB::Types::SystemColumn::get_meta_cid_str(colp->get_sequence()));
  }

  SWC_LOCATOR_REQ_DEBUG("rgr_locate");

  Comm::Protocol::Rgr::Req::RangeLocate::request(
    hdlr->clients,
    params, endpoints,
    [profile=hdlr->profile.rgr_locate(type), committer=shared_from_this()]
    (const ReqBase::Ptr& req,
     const Comm::Protocol::Rgr::Params::RangeLocateRsp& rsp) {
      profile.add(!rsp.rid || rsp.err);
      committer->located_on_ranger(
        std::dynamic_pointer_cast<
          Comm::Protocol::Rgr::Req::RangeLocate>(req)->endpoints,
        req, rsp
      );
    }
  );
}

void Committer::located_on_ranger(
      const Comm::EndPoints& endpoints,
      const ReqBase::Ptr& base,
      const Comm::Protocol::Rgr::Params::RangeLocateRsp& rsp) {
  if(!hdlr->valid())
    return hdlr->response();
  switch(rsp.err) {
    case Error::OK:
      break;
    case Error::RANGE_NOT_FOUND:
    case Error::RGR_NOT_LOADED_RANGE:
    case Error::SERVER_SHUTTING_DOWN:
    case Error::COMM_NOT_CONNECTED: {
      SWC_LOCATOR_RSP_DEBUG("rgr_located RETRYING");
      hdlr->clients->rgr_cache_remove(cid, rid);
      return parent->request_again();
    }
    default: {
      SWC_LOCATOR_RSP_DEBUG("rgr_located RETRYING");
      if(rsp.err != Error::REQUEST_TIMEOUT)
        hdlr->clients->rgr_cache_remove(cid, rid);
      return base->request_again();
    }
  }
  if(!rsp.rid) {
    SWC_LOCATOR_RSP_DEBUG("rgr_located RETRYING(no-rid)");
    return base->request_again();
  }

  SWC_LOCATOR_RSP_DEBUG("rgr_located");

  std::make_shared<Committer>(
    type == DB::Types::Range::MASTER
            ? DB::Types::Range::META
            : DB::Types::Range::DATA,
    rsp.cid, colp, key_start,
    hdlr, base,
    rsp.rid, rsp.range_end
  )->resolve_on_manager();

  if(!rsp.range_end.empty()) {
    auto next_key_start = colp->get_key_next(rsp.range_end);
    if(next_key_start) {
      std::make_shared<Committer>(
        type,
        cid, colp, next_key_start,
        hdlr, base,
        rid
      )->locate_on_ranger(endpoints);
    }
  }
  hdlr->response();
}

void Committer::resolve_on_manager() {
  hdlr->completion.increment();
  if(!hdlr->valid())
    return hdlr->response();

  Comm::Protocol::Mngr::Params::RgrGetReq params(cid, rid);
  auto req = Comm::Protocol::Mngr::Req::RgrGet::make(
    hdlr->clients,
    params,
    [profile=hdlr->profile.mngr_res(), committer=shared_from_this()]
    (const ReqBase::Ptr& req,
     const Comm::Protocol::Mngr::Params::RgrGetRsp& rsp) {
      profile.add(rsp.err || !rsp.rid || rsp.endpoints.empty());
      committer->located_ranger(req, rsp);
    }
  );
  if(!DB::Types::SystemColumn::is_master(cid)) {
    auto profile = hdlr->profile.mngr_res();
    Comm::Protocol::Mngr::Params::RgrGetRsp rsp(cid, rid);
    if(hdlr->clients->rgr_cache_get(cid, rid, rsp.endpoints)) {
      profile.add_cached(Error::OK);
      SWC_LOCATOR_RSP_DEBUG("mngr_resolve_rgr Cache hit");
      return proceed_on_ranger(req, rsp);
    }
  }
  SWC_LOCATOR_REQ_DEBUG("mngr_resolve_rgr");
  req->run();
}

void Committer::located_ranger(
      const ReqBase::Ptr& base,
      const Comm::Protocol::Mngr::Params::RgrGetRsp& rsp) {
  if(!hdlr->valid())
    return hdlr->response();
  switch(rsp.err) {
    case Error::OK:
      break;
    case Error::COLUMN_NOT_EXISTS: {
      SWC_LOCATOR_RSP_DEBUG("rgr_located");
      if(cid == colp->get_cid())
        hdlr->error(cid, rsp.err);
      return hdlr->response(rsp.err);
    }
    case Error::CLIENT_STOPPING: {
      SWC_LOCATOR_RSP_DEBUG("rgr_located STOPPED");
      hdlr->error(rsp.err);
      return hdlr->response(rsp.err);
    }
    default: {
      SWC_LOCATOR_RSP_DEBUG("rgr_located RETRYING");
      return (parent && rsp.err == Error::RANGE_NOT_FOUND
              ? parent : base)->request_again();
    }
  }
  if(!rsp.rid || rsp.endpoints.empty()) {
    SWC_LOCATOR_RSP_DEBUG("rgr_located RETRYING(no-rid)");
    return (parent ? parent : base)->request_again();
  }

  SWC_LOCATOR_RSP_DEBUG("rgr_located");

  if(!DB::Types::SystemColumn::is_master(rsp.cid))
    hdlr->clients->rgr_cache_set(rsp.cid, rsp.rid, rsp.endpoints);

  proceed_on_ranger(base, rsp);
}

void Committer::proceed_on_ranger(
      const ReqBase::Ptr& base,
      const Comm::Protocol::Mngr::Params::RgrGetRsp& rsp) {
  if(!hdlr->valid())
    return hdlr->response();
  switch(type) {
    case DB::Types::Range::MASTER: {
      if(!DB::Types::SystemColumn::is_master(colp->get_cid()))
        goto do_next_locate;
      break;
    }
    case DB::Types::Range::META: {
      if(!DB::Types::SystemColumn::is_meta(colp->get_cid()))
        goto do_next_locate;
      break;
    }
    case DB::Types::Range::DATA: {
      break;
    }
  }

  if(cid != rsp.cid || colp->get_cid() != cid) {
    hdlr->clients->rgr_cache_remove(cid, rid);
    SWC_LOCATOR_RSP_DEBUG("rgr_located RETRYING(cid no match)");
    return (parent ? parent : base)->request_again();;
  }
  commit_data(rsp.endpoints, base);
  return hdlr->response();

  do_next_locate: {
    (DB::Types::SystemColumn::is_master(cid)
      ? std::make_shared<Committer>(
          type,
          rsp.cid, colp, key_start,
          hdlr, base,
          rsp.rid, rsp.range_end)
      : std::make_shared<Committer>(
          type,
          rsp.cid, colp, key_start,
          hdlr, base,
          rsp.rid)
    )->locate_on_ranger(rsp.endpoints);
    return hdlr->response();
  }
}

void Committer::commit_data(
      const Comm::EndPoints& endpoints,
      const ReqBase::Ptr& base) {
  hdlr->completion.increment();

  bool more = true;
  DynamicBuffer::Ptr cells_buff;
  auto workload = std::make_shared<Core::CompletionCounter<>>(1);

  while(more && hdlr->valid() &&
        (cells_buff = colp->get_buff(
          *key_start.get(), key_finish, hdlr->buff_sz, more))) {
    workload->increment();

    Comm::Protocol::Rgr::Req::RangeQueryUpdate::request(
      hdlr->clients,
      Comm::Protocol::Rgr::Params::RangeQueryUpdateReq(colp->get_cid(), rid),
      cells_buff,
      endpoints,
      [workload, cells_buff, base,
       profile=hdlr->profile.rgr_data(), committer=shared_from_this()]
      (ReqBase::Ptr req,
       const Comm::Protocol::Rgr::Params::RangeQueryUpdateRsp& rsp) {
        profile.add(rsp.err);
        switch(rsp.err) {

          case Error::OK: {
            SWC_UPDATER_LOCATOR_RSP_DEBUG("rgr_commit");
            if(workload->is_last())
              committer->hdlr->response();
            return;
          }

          case Error::REQUEST_TIMEOUT: {
            SWC_UPDATER_LOCATOR_RSP_DEBUG("rgr_commit RETRYING");
            return req->request_again();
          }

          case Error::RANGE_BAD_INTERVAL:
          case Error::RANGE_BAD_CELLS_INPUT: {
            SWC_UPDATER_LOCATOR_RSP_DEBUG("rgr_commit RETRYING");
            size_t resend = committer->colp->add(
              *cells_buff.get(), rsp.range_prev_end, rsp.range_end,
              rsp.cells_added, rsp.err == Error::RANGE_BAD_CELLS_INPUT);
            committer->hdlr->add_resend_count(resend);

            if(workload->is_last()) {
              auto nxt_start = committer->colp->get_key_next(rsp.range_end);
              if(nxt_start) {
                std::make_shared<Committer>(
                  DB::Types::Range::MASTER,
                  committer->colp->get_cid(), committer->colp, nxt_start,
                  committer->hdlr
                )->locate_on_manager();
               }
              committer->hdlr->response();
            }
            return;
          }

          default: {
            committer->hdlr->add_resend_count(
              committer->colp->add(*cells_buff.get())
            );
            if(workload->is_last()) {
              SWC_UPDATER_LOCATOR_RSP_DEBUG("rgr_commit RETRYING");
              base->request_again();
            }
            return;
          }

        }
      },

      hdlr->timeout + cells_buff->fill()/hdlr->timeout_ratio
    );

  }

  if(workload->is_last())
    hdlr->response();
}



}}}}
