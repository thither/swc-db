/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swcdb_core_comm_HeaderBufferInfo_h
#define swcdb_core_comm_HeaderBufferInfo_h


#include "swcdb/core/Compat.h"
#include "swcdb/core/Encoder.h"


namespace SWC { namespace Comm {


struct BufferInfo final {

  BufferInfo() noexcept;

  void reset() noexcept;

  uint8_t encoded_length() const noexcept;

  void encode(uint8_t** bufp) const;

  void decode(const uint8_t** bufp, size_t* remainp);

  void encode(Core::Encoder::Type encoder, StaticBuffer& data);

  void decode(int& err, StaticBuffer& data) const;

  void print(std::ostream& out) const;

  uint32_t            size;        //!< Buffer size
  Core::Encoder::Type encoder;     //!< Buffer Encoder
  uint32_t            size_plain;  //!< Buffer set if Encoder not PLAIN
  uint32_t            chksum;      //!< Buffer checksum

} __attribute__((packed));


}}



#ifdef SWC_IMPL_SOURCE
#include "swcdb/core/comm/HeaderBufferInfo.cc"
#endif

#endif // swcdb_core_comm_HeaderBufferInfo_h
