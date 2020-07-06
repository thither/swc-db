/*
 * Copyright Since 2019 SWC-DB© [author: Kashirin Alex kashirin.alex@gmail.com]
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swc_fs_Broker_Protocol_req_Read_h
#define swc_fs_Broker_Protocol_req_Read_h

#include "swcdb/fs/Broker/Protocol/req/Base.h"
#include "swcdb/fs/Broker/Protocol/params/Read.h"

namespace SWC { namespace FS { namespace Protocol { namespace Req {

class Read : public Base {

  public:
  
  void*   buffer;
  bool    allocated;
  size_t  amount;
  
  Read(uint32_t timeout, SmartFd::Ptr& smartfd, void* dst, size_t len, 
       bool allocated, const Callback::ReadCb_t& cb=0)
      : buffer(dst), allocated(allocated), amount(0), 
        smartfd(smartfd), cb(cb) {
    SWC_LOGF(LOG_DEBUG, "read len=%lu timeout=%d %s", 
              len, timeout, smartfd->to_string().c_str());

    cbp = CommBuf::make(Params::ReadReq(smartfd->fd(), len));
    cbp->header.set(Cmd::FUNCTION_READ, timeout);
  }

  std::promise<void> promise() {
    std::promise<void>  r_promise;
    cb = [await=&r_promise]
         (int, const SmartFd::Ptr&, const StaticBuffer::Ptr&) {
           await->set_value();
          };
    return r_promise;
  }

  void handle(ConnHandlerPtr, const Event::Ptr& ev) override {

    const uint8_t *ptr;
    size_t remain;

    if(!Base::is_rsp(ev, Cmd::FUNCTION_READ, &ptr, &remain))
      return;

    StaticBuffer::Ptr buf = nullptr;
    switch(error) {
      case Error::OK:
      case Error::FS_EOF: {
        Params::ReadRsp params;
        params.decode(&ptr, &remain);
        amount = ev->data_ext.size;
        smartfd->pos(params.offset + amount);
        if(amount) {
          if(buffer == nullptr) {
            buf = std::make_shared<StaticBuffer>(ev->data_ext);
          } else {
            if(allocated)
              memcpy(buffer, ev->data_ext.base, amount);
            else
              ((StaticBuffer*)buffer)->set(ev->data_ext);
          }
        }
        break;
      }
      case EBADR:
      case Error::FS_BAD_FILE_HANDLE:
        smartfd->fd(-1);
      default:
        break;
    }

    SWC_LOGF(LOG_DEBUG, "read %s amount='%lu' error='%d'", 
              smartfd->to_string().c_str(), amount, error);

    cb(error, smartfd, buf);
  }

  private:
  SmartFd::Ptr        smartfd;
  Callback::ReadCb_t  cb;
};



}}}}

#endif  // swc_fs_Broker_Protocol_req_Read_h