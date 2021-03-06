/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swcdb_fs_Broker_AppContext_h
#define swcdb_fs_Broker_AppContext_h

#include "swcdb/core/comm/AppContext.h"

namespace SWC { namespace client {



/**
 * @brief The SWC-DB FS::Broker's Client to FsBroker C++ namespace 'SWC::client::FsBroker'
 *
 * \ingroup FileSystem
 */
namespace FsBroker {


class AppContext final : public Comm::AppContext {
  public:

  AppContext(const Config::Settings& settings)
      : Comm::AppContext(
          settings.get<Config::Property::V_GENUM>(
            "swc.fs.broker.comm.encoder")) {
  }

  virtual ~AppContext() { }

  void handle(Comm::ConnHandlerPtr, const Comm::Event::Ptr& ev) override {

    switch(ev->type) {
      case Comm::Event::Type::DISCONNECT:
      case Comm::Event::Type::ESTABLISHED:
        return;
      default: {
        /*
        const uint8_t *ptr;
        size_t remain;
        if(Comm::Protocol::FsBroker::Req::Base().is_rsp(
            ev, ev->header.command, &ptr, &remain))
        */
        SWC_LOG_OUT(LOG_WARN,  ev->print(SWC_LOG_OSTREAM << "Unhandled "); );
        break;
      }
    }

  }
};

}}}

#endif // swcdb_fs_Broker_AppContext_h
