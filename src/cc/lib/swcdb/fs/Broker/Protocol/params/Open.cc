/*
 * Copyright Since 2019 SWC-DB© [author: Kashirin Alex kashirin.alex@gmail.com]
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#include "swcdb/fs/Broker/Protocol/params/Open.h"


namespace SWC { namespace FS { namespace Protocol { namespace Params {


OpenReq::OpenReq() { }

OpenReq::OpenReq(const std::string& fname, uint32_t flags, int32_t bufsz)
                : fname(fname), flags(flags), bufsz(bufsz) { }

size_t OpenReq::internal_encoded_length() const {
  return Serialization::encoded_length_vi32(flags)
       + Serialization::encoded_length_vi32(bufsz) 
       + Serialization::encoded_length_bytes(fname.size());
}

void OpenReq::internal_encode(uint8_t** bufp) const {
  Serialization::encode_vi32(bufp, flags);
  Serialization::encode_vi32(bufp, bufsz);
  Serialization::encode_bytes(bufp, fname.c_str(), fname.size());
}

void OpenReq::internal_decode(const uint8_t** bufp, size_t* remainp) {
  flags = Serialization::decode_vi32(bufp, remainp);
  bufsz = Serialization::decode_vi32(bufp, remainp);
  fname.clear();
  fname.append(Serialization::decode_bytes_string(bufp, remainp));
}



OpenRsp::OpenRsp(): fd(-1) {}

OpenRsp::OpenRsp(int32_t fd): fd(fd) {}

size_t OpenRsp::internal_encoded_length() const {
  return Serialization::encoded_length_vi32(fd);
}

void OpenRsp::internal_encode(uint8_t** bufp) const {
  Serialization::encode_vi32(bufp, fd);
}

void OpenRsp::internal_decode(const uint8_t** bufp, size_t* remainp) {
  fd = Serialization::decode_vi32(bufp, remainp);
}


}}}}