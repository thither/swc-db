/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swc_manager_AppContext_h
#define swc_manager_AppContext_h

#include "swcdb/db/Protocol/Commands.h"

#include "swcdb/core/comm/AppContext.h"
#include "swcdb/core/comm/AppHandler.h"
#include "swcdb/core/comm/ResponseCallback.h"
#include "swcdb/core/comm/DispatchHandler.h"

#include "swcdb/fs/Interface.h"
#include "swcdb/db/client/Clients.h"

#include "swcdb/manager/AppContextClient.h"
#include "swcdb/manager/MngrEnv.h"

#include "swcdb/common/Protocol/handlers/NotImplemented.h"
#include "swcdb/manager/Protocol/handlers/MngrState.h"
#include "swcdb/manager/Protocol/handlers/MngrActive.h"
#include "swcdb/manager/Protocol/handlers/ColumnMng.h"
#include "swcdb/manager/Protocol/handlers/ColumnUpdate.h"
#include "swcdb/manager/Protocol/handlers/ColumnGet.h"
#include "swcdb/manager/Protocol/handlers/ColumnList.h"
#include "swcdb/manager/Protocol/handlers/ColumnCompact.h"
#include "swcdb/manager/Protocol/handlers/RgrMngId.h"
#include "swcdb/manager/Protocol/handlers/RgrUpdate.h"
#include "swcdb/manager/Protocol/handlers/RgrGet.h"
#include "swcdb/manager/Protocol/handlers/RangeCreate.h"
#include "swcdb/manager/Protocol/handlers/RangeUnloaded.h"
#include "swcdb/manager/Protocol/handlers/RangeRemove.h"
#include "swcdb/manager/Protocol/handlers/Report.h"
#include "swcdb/manager/Protocol/handlers/Echo.h"


namespace SWC { namespace Manager {


class AppContext final : public SWC::Comm::AppContext {
   
  // in-order of Protocol::Mngr::Command
  static constexpr const Comm::AppHandler_t handlers[] = { 
    &Protocol::Common::Handler::not_implemented,
    &Protocol::Mngr::Handler::mngr_state,
    &Protocol::Mngr::Handler::mngr_active,
    &Protocol::Mngr::Handler::column_mng,
    &Protocol::Mngr::Handler::column_update,
    &Protocol::Mngr::Handler::column_get,
    &Protocol::Mngr::Handler::column_list,
    &Protocol::Mngr::Handler::column_compact,
    &Protocol::Mngr::Handler::rgr_mng_id,
    &Protocol::Mngr::Handler::rgr_update,
    &Protocol::Mngr::Handler::rgr_get,
    &Protocol::Mngr::Handler::range_create,
    &Protocol::Mngr::Handler::range_unloaded,
    &Protocol::Mngr::Handler::range_remove,
    &Protocol::Mngr::Handler::report,
    &Protocol::Mngr::Handler::do_echo,
    //&Handler::debug,
    //&Handler::status,
    //&Handler::shutdown
  };

  public:

  AppContext() {
    auto settings = Env::Config::settings();

    settings->parse_file(
      settings->get_str("swc.mngr.cfg", ""), 
      "swc.mngr.cfg.dyn"
    );

    Env::IoCtx::init(settings->get_i32("swc.mngr.handlers"));

    Env::FsInterface::init(FS::fs_type(settings->get_str("swc.fs")));
      
    Env::Clients::init(
      std::make_shared<client::Clients>(
        Env::IoCtx::io()->shared(),
        std::make_shared<client::Mngr::AppContext>()
      )
    );

    auto period = settings->get<Config::Property::V_GINT32>(
      "swc.cfg.dyn.period");
    if(period->get()) {
      Env::IoCtx::io()->set_periodic_timer(
        period,
        [](){Env::Config::settings()->check_dynamic_files();}
      );
    }
  }
  
  void init(const Comm::EndPoints& endpoints) override {
    Env::Mngr::init(endpoints);
    
    int sig = 0;
    Env::IoCtx::io()->set_signals();
    shutting_down(std::error_code(), sig);
  }

  void set_srv(Comm::server::SerializedServer::Ptr srv){
    m_srv = srv;
  }

  virtual ~AppContext(){}


  void handle(Comm::ConnHandlerPtr conn, const Comm::Event::Ptr& ev) override {
    // SWC_LOGF(LOG_DEBUG, "handle: %s", ev->to_str().c_str());

    switch (ev->type) {

      case Comm::Event::Type::ESTABLISHED:
        m_srv->connection_add(conn);
        return; 
        
      case Comm::Event::Type::DISCONNECT:
        m_srv->connection_del(conn);
        Env::Mngr::role()->disconnection(
          conn->endpoint_remote, conn->endpoint_local, true);
        return;

      case Comm::Event::Type::ERROR:
        //rangers->decommision(event->addr);
        break;

      case Comm::Event::Type::MESSAGE: {
        uint8_t cmd = ev->header.command >= Protocol::Mngr::MAX_CMD
                        ? (uint8_t)Protocol::Mngr::NOT_IMPLEMENTED 
                        : ev->header.command;
        Env::IoCtx::post([cmd, conn, ev]() { handlers[cmd](conn, ev); });
        return;
      }

      default:
        SWC_LOGF(LOG_WARN, "Unimplemented event-type (%d)", (int)ev->type);
        break;

    }
  }
  
  void shutting_down(const std::error_code &ec, const int &sig) {

    if(sig == 0) { // set signals listener
      Env::IoCtx::io()->signals()->async_wait(
        [this](const std::error_code &ec, const int &sig) {
          SWC_LOGF(LOG_INFO, "Received signal, sig=%d ec=%s", sig, ec.message().c_str());
          shutting_down(ec, sig); 
        }
      ); 
      SWC_LOGF(LOG_INFO, "Listening for Shutdown signal, set at sig=%d ec=%s", 
              sig, ec.message().c_str());
      return;
    }

    SWC_LOGF(LOG_INFO, "Shutdown signal, sig=%d ec=%s", sig, ec.message().c_str());
    
    if(m_srv == nullptr) {
      SWC_LOG(LOG_INFO, "Exit");
      std::quick_exit(0);
    }
    
    (new std::thread([ptr=shared_from_this()]{ ptr->stop(); }))->detach();
  }

  void stop() override {
    
    m_srv->stop_accepting(); // no further requests accepted
    
    Env::Mngr::stop();

    Env::Clients::get()->rgr->stop();
    Env::Clients::get()->mngr->stop();

    Env::IoCtx::io()->stop();
    Env::FsInterface::interface()->stop();
    
    m_srv->shutdown();
    
    SWC_LOG(LOG_INFO, "Exit");
    std::quick_exit(0);
  }

  private:
  Comm::server::SerializedServer::Ptr m_srv = nullptr;
  //ColmNameToIDMap columns;       // column-name > CID


};

}}

#endif // swc_manager_AppContext_h