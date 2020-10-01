/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swcdb_fs_Broker_Protocol_req_Seek_h
#define swcdb_fs_Broker_Protocol_req_Seek_h

#include "swcdb/fs/Broker/Protocol/req/Base.h"
#include "swcdb/fs/Broker/Protocol/params/Seek.h"

namespace SWC { namespace FsBroker { namespace Protocol { namespace Req {

class Seek : public Base {

  public:
  
  Seek(uint32_t timeout, FS::SmartFd::Ptr& smartfd, size_t offset,
       const FS::Callback::SeekCb_t& cb=0);

  std::promise<void> promise();

  void handle(Comm::ConnHandlerPtr, const Comm::Event::Ptr& ev) override;

  private:
  FS::SmartFd::Ptr        smartfd;
  FS::Callback::SeekCb_t  cb;

};



}}}}


#ifdef SWC_IMPL_SOURCE
#include "swcdb/fs/Broker/Protocol/req/Seek.cc"
#endif 

#endif // swcdb_fs_Broker_Protocol_req_Seek_h
