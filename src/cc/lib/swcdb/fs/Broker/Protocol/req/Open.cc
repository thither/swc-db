/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#include "swcdb/fs/Broker/Protocol/req/Open.h"


namespace SWC { namespace Comm { namespace Protocol {
namespace FsBroker {  namespace Req {


Open::Open(FS::FileSystem::Ptr fs, uint32_t timeout, FS::SmartFd::Ptr& smartfd, 
           int32_t bufsz, const FS::Callback::OpenCb_t& cb) 
          : fs(fs), smartfd(smartfd), cb(cb) {
  SWC_LOG_OUT(LOG_DEBUG,
    SWC_LOG_PRINTF("open timeout=%d ", timeout);
    smartfd->print(SWC_LOG_OSTREAM); 
  );

  cbp = Buffers::make(
    Params::OpenReq(smartfd->filepath(), smartfd->flags(), bufsz));
  cbp->header.set(FUNCTION_OPEN, timeout);
}

std::promise<void> Open::promise() {
  std::promise<void>  r_promise;
  cb = [await=&r_promise](int, const FS::SmartFd::Ptr&){ await->set_value(); };
  return r_promise;
}

void Open::handle(ConnHandlerPtr, const Event::Ptr& ev) { 

  const uint8_t *ptr;
  size_t remain;

  if(!Base::is_rsp(ev, FUNCTION_OPEN, &ptr, &remain))
    return;

  if(!error) {
    try {
      Params::OpenRsp params;
      params.decode(&ptr, &remain);
      smartfd->fd(params.fd);
      smartfd->pos(0);
      fs->fd_open_incr();

    } catch(...) {
      const Error::Exception& e = SWC_CURRENT_EXCEPTION("");
      error = e.code();
    }
  }

  SWC_LOG_OUT(LOG_DEBUG, 
    SWC_LOG_PRINTF("open fds-open=%lu ", fs->fds_open());
    Error::print(SWC_LOG_OSTREAM, error);
    smartfd->print(SWC_LOG_OSTREAM << ' ');
  );
  
  cb(error, smartfd);
}



}}}}}
