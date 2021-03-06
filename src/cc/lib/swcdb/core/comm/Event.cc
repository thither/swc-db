/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */



#include "swcdb/core/Exception.h"
#include "swcdb/core/Serialization.h"
#include "swcdb/core/comm/Event.h"


namespace SWC { namespace Comm {



void Event::decode_buffers() {
  int err = Error::OK;
  int n = 1;
  header.data.decode(err, data);
  if(!err && header.buffers > 1) {
    ++n;
    header.data_ext.decode(err, data_ext);
  }

  if(err) {
    type = Event::Type::ERROR;
    error = Error::REQUEST_TRUNCATED_PAYLOAD;
    data.free();
    data_ext.free();
    SWC_LOG_OUT(LOG_WARN,
      SWC_LOG_OSTREAM << "decode, REQUEST ENCODER_DECODE: n(" << n << ") ";
      print(SWC_LOG_OSTREAM);
    );
  }
}

bool Event::expired(int64_t within) const noexcept {
  return expiry_ms && Time::now_ms() > (expiry_ms - within);
}

int32_t Event::response_code() {
  if(error || type == Event::ERROR)
    return error;

  const uint8_t *ptr = data.base;
  size_t remaining = data.size;

  try {
    return Serialization::decode_i32(&ptr, &remaining);
    /* opt
    std::string msg = Serialization::decode_bytes_string(&ptr, &remaining);
    */
  } catch(...) {
    const Error::Exception& e = SWC_CURRENT_EXCEPTION("");
    SWC_LOG_OUT(LOG_ERROR, SWC_LOG_OSTREAM << e; );
    return e.code();
  }
}

void Event::print(std::ostream& out) const {
  out << "Event: type=";
  switch(type){
  case ESTABLISHED:
    out << "ESTABLISHED";
    break;
  case DISCONNECT:
    out << "DISCONNECT";
    break;
  case MESSAGE:
    header.print(out << "MESSAGE ");
    out << " buffers-sz(" << data.size << ',' << data_ext.size << ')';
    break;
  case ERROR:
    out << "ERROR";
    break;
  default:
    out << "UKNOWN(" << int(type) << ')';
    break;
  }
  if(error)
    Error::print(out << ' ', error);
}


}} // namespace SWC::Comm

