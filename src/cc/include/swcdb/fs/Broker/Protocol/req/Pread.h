/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swcdb_fs_Broker_Protocol_req_Pread_h
#define swcdb_fs_Broker_Protocol_req_Pread_h

#include "swcdb/fs/Broker/Protocol/req/Base.h"
#include "swcdb/fs/Broker/Protocol/params/Pread.h"

namespace SWC { namespace Comm { namespace Protocol {
namespace FsBroker {  namespace Req {

class Pread : public Base {

  public:
  
  void*   buffer;
  bool    allocated;
  size_t  amount;
  
  Pread(uint32_t timeout, FS::SmartFd::Ptr& smartfd, 
        uint64_t offset, void* dst, size_t len, bool allocated,
        const FS::Callback::PreadCb_t& cb=0);

  std::promise<void> promise();

  void handle(ConnHandlerPtr, const Event::Ptr& ev) override;

  private:
  FS::SmartFd::Ptr        smartfd;
  FS::Callback::PreadCb_t cb;

};



}}}}}


#ifdef SWC_IMPL_SOURCE
#include "swcdb/fs/Broker/Protocol/req/Pread.cc"
#endif 

#endif // swcdb_fs_Broker_Protocol_req_Pread_h
