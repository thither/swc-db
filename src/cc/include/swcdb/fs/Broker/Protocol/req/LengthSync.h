/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swcdb_fs_Broker_Protocol_req_LengthSync_h
#define swcdb_fs_Broker_Protocol_req_LengthSync_h

#include "swcdb/fs/Broker/Protocol/req/Base.h"
#include "swcdb/fs/Broker/Protocol/params/Length.h"


namespace SWC { namespace Comm { namespace Protocol {
namespace FsBroker {  namespace Req {


class LengthSync final : public BaseSync, public Base {
  public:

  size_t length;

  LengthSync(FS::Statistics& stats,
             uint32_t timeout, const std::string& name)
            : Base(
                stats, FS::Statistics::LENGTH_SYNC,
                Buffers::make(
                  Params::LengthReq(name),
                  0,
                  FUNCTION_LENGTH, timeout
                )
              ),
              length(0), name(name) {
  }

  void handle(ConnHandlerPtr, const Event::Ptr& ev) override {
    Base::handle_length(ev, name, length);
    BaseSync::acknowledge();
  }

  private:
  const std::string name;

};


}}}}}


#endif // swcdb_fs_Broker_Protocol_req_LengthSync_h
