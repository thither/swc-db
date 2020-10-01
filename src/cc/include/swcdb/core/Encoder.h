/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swcdb_core_Encoder_h
#define swcdb_core_Encoder_h

#include "swcdb/core/Buffer.h"

namespace SWC { namespace Encoder {

enum class Type {
  DEFAULT = 0x00,
  PLAIN   = 0x01,
  ZLIB    = 0x02,
  SNAPPY  = 0x03,
  ZSTD    = 0x04,
  UNKNOWN = 0xff
};

std::string to_string(Type typ);

Type encoding_from(const std::string& typ);

std::string repr_encoding(int typ);

int from_string_encoding(const std::string& typ);



void decode(int& err, Type encoder, 
            const uint8_t* src, size_t sz_enc, 
            uint8_t *dst, size_t sz);

void encode(int& err, Type encoder, 
            const uint8_t* src, size_t src_sz, 
            size_t* sz_enc, DynamicBuffer& output, 
            uint32_t reserve);

}}


#ifdef SWC_IMPL_SOURCE
#include "swcdb/core/Encoder.cc"
#endif 


#endif // swcdb_core_Encoder_h
