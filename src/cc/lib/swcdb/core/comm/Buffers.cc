/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#include "swcdb/core/comm/Buffers.h"
#include "swcdb/core/Serialization.h"
#include "swcdb/core/Checksum.h"

namespace SWC { namespace Comm {

namespace {
static const uint16_t BUFFER_CHUNK_SZ = 4096;
}


SWC_SHOULD_INLINE
Buffers::Ptr
Buffers::create_error_message(const Event::Ptr& ev,
                              int error, const char *msg, uint16_t len) {
  auto cbp = Buffers::make(ev, 4 + Serialization::encoded_length_bytes(len));
  Serialization::encode_i32(&cbp->data_ptr, error);
  Serialization::encode_bytes(&cbp->data_ptr, msg, len);
  return cbp;
}


/* Init Common */
Buffers::Buffers(uint32_t reserve)
                : expiry_ms(0) {
  if(reserve)
    set_data(reserve);
}

Buffers::Buffers(const Serializable& params, uint32_t reserve)
                : expiry_ms(0) {
  set_data(params, reserve);
}

Buffers::Buffers(const Serializable& params, StaticBuffer& buffer,
                 uint32_t reserve)
                : expiry_ms(0), buf_ext(buffer) {
  set_data(params, reserve);
}

Buffers::Buffers(StaticBuffer& buffer, uint32_t reserve)
                : expiry_ms(0), buf_ext(buffer) {
  if(reserve)
    set_data(reserve);
}


/* Init Request */
Buffers::Buffers(const Serializable& params, uint32_t reserve,
                 uint64_t cmd, uint32_t timeout)
                : header(cmd, timeout), expiry_ms(0) {
  set_data(params, reserve);
}

Buffers::Buffers(const Serializable& params, StaticBuffer& buffer,
                 uint32_t reserve,
                 uint64_t cmd, uint32_t timeout)
                : header(cmd, timeout), expiry_ms(0),
                  buf_ext(buffer) {
  set_data(params, reserve);
}


/* Init Response */
Buffers::Buffers(const Event::Ptr& ev, uint32_t reserve)
                : header(ev->header), expiry_ms(ev->expiry_ms) {
  if(reserve)
    set_data(reserve);
}

Buffers::Buffers(const Event::Ptr& ev,
                 const Serializable& params, uint32_t reserve)
                : header(ev->header), expiry_ms(ev->expiry_ms) {
  set_data(params, reserve);
}

Buffers::Buffers(const Event::Ptr& ev,
                 const Serializable& params, StaticBuffer& buffer,
                 uint32_t reserve)
                : header(ev->header), expiry_ms(ev->expiry_ms),
                  buf_ext(buffer) {
  set_data(params, reserve);
}

Buffers::Buffers(const Event::Ptr& ev,
                 StaticBuffer& buffer, uint32_t reserve)
                : header(ev->header), expiry_ms(ev->expiry_ms),
                  buf_ext(buffer) {
  if(reserve)
    set_data(reserve);
}

void Buffers::set_data(uint32_t sz) {
  if(buf_data.size)
    header.data.reset();

  buf_data.reallocate(sz);
  data_ptr = buf_data.base;
}

void Buffers::set_data(const Serializable& params, uint32_t reserve) {
  set_data(reserve + params.encoded_length());

  data_ptr = buf_data.base + reserve;
  params.encode(&data_ptr);
  data_ptr = buf_data.base;
}

void Buffers::prepare(Core::Encoder::Type encoder) {
  if(buf_data.size) {
    header.data.encode(encoder, buf_data);
    if(buf_ext.size)
      header.data_ext.encode(encoder, buf_ext);
  }
}

SWC_SHOULD_INLINE
bool Buffers::expired() const noexcept {
  return expiry_ms && Time::now_ms() > expiry_ms;
}

SWC_SHOULD_INLINE
uint8_t Buffers::write_header() {
  uint8_t len = header.encoded_length();
  uint8_t* buf = buf_header;
  header.encode(&buf);
  return len;
}

std::vector<asio::const_buffer> Buffers::get_buffers() {
  uint8_t buf_header_len = write_header();

  size_t nchunks = 1;

  size_t buf_data_chunks = 0;
  bool buf_data_not_aligned = false;
  if(buf_data.size)
    nchunks += (buf_data_chunks += buf_data.size / BUFFER_CHUNK_SZ)
            + (buf_data_not_aligned = buf_data.size % BUFFER_CHUNK_SZ);

  size_t buf_ext_chunks = 0;
  bool buf_ext_not_aligned = false;
  if(buf_ext.size)
    nchunks += (buf_ext_chunks += buf_ext.size / BUFFER_CHUNK_SZ)
            + (buf_ext_not_aligned = buf_ext.size % BUFFER_CHUNK_SZ);

  std::vector<asio::const_buffer> buffers(nchunks);
  auto it = buffers.begin();
  *it = asio::const_buffer(buf_header, buf_header_len);

  if(buf_data.size) {
    auto p = buf_data.base;
    for(size_t i = 0; i<buf_data_chunks; ++i, p+=BUFFER_CHUNK_SZ)
      *++it = asio::const_buffer(p, BUFFER_CHUNK_SZ);
    if(buf_data_not_aligned)
      *++it = asio::const_buffer(p, (buf_data.base + buf_data.size) - p);
  }
  if(buf_ext.size) {
    auto p = buf_ext.base;
    for(size_t i = 0; i<buf_ext_chunks; ++i, p+=BUFFER_CHUNK_SZ)
      *++it = asio::const_buffer(p, BUFFER_CHUNK_SZ);
    if(buf_ext_not_aligned)
      *++it = asio::const_buffer(p, (buf_ext.base + buf_ext.size) - p);
  }
  return buffers;
}

SWC_SHOULD_INLINE
void Buffers::append_i8(uint8_t ival) noexcept {
  Serialization::encode_i8(&data_ptr, ival);
}

SWC_SHOULD_INLINE
void Buffers::append_i32(uint32_t ival) noexcept {
  Serialization::encode_i32(&data_ptr, ival);
}


}} // namespace SWC::Comm
