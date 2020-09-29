/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#include "swcdb/manager/Settings.h"
#include "swcdb/core/comm/SerializedServer.h"
#include "swcdb/manager/AppContext.h"


namespace SWC {

int run() {
  SWC_TRY_OR_LOG("", 
  
  auto app_ctx  = std::make_shared<Manager::AppContext>();

  auto srv = std::make_shared<Comm::server::SerializedServer>(
    "MANAGER", 
    Env::Config::settings()->get_i32("swc.mngr.reactors"), 
    Env::Config::settings()->get_i32("swc.mngr.workers"), 
    "swc.mngr.port",
    app_ctx
  );
  app_ctx->set_srv(srv);
  srv->run();

  return 0);
  return 1;
}

}

int main(int argc, char** argv) {
  SWC::Env::Config::init(argc, argv);
  SWC::Env::Config::settings()->init_process();
  return SWC::run();
}
