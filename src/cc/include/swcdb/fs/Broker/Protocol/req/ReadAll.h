/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swc_fs_Broker_Protocol_req_ReadAll_h
#define swc_fs_Broker_Protocol_req_ReadAll_h

#include "swcdb/fs/Broker/Protocol/req/Base.h"
#include "swcdb/fs/Broker/Protocol/params/ReadAll.h"

namespace SWC { namespace FS { namespace Protocol { namespace Req {

class ReadAll : public Base {

  public:
  StaticBuffer* buffer;
  
  ReadAll(uint32_t timeout, const std::string& name, StaticBuffer* dst,
          const Callback::ReadAllCb_t& cb=0);

  std::promise<void> promise();

  void handle(Comm::ConnHandlerPtr, const Comm::Event::Ptr& ev);

  private:
  const std::string      name;
  Callback::ReadAllCb_t  cb;

};



}}}}


#ifdef SWC_IMPL_SOURCE
#include "swcdb/fs/Broker/Protocol/req/ReadAll.cc"
#endif 

#endif  // swc_fs_Broker_Protocol_req_ReadAll_h
