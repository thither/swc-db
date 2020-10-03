/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#include "swcdb/core/Exception.h"
#include "swcdb/core/Encoder.h"

#include <snappy.h>
#include <zlib.h>
#include <zstd.h>

namespace SWC { namespace Core { namespace Encoder {


namespace {
  const char Encoder_DEFAULT[]  = "DEFAULT";
  const char Encoder_PLAIN[]    = "PLAIN";
  const char Encoder_ZLIB[]     = "ZLIB";
  const char Encoder_SNAPPY[]   = "SNAPPY";
  const char Encoder_ZSTD[]     = "ZSTD";
  const char Encoder_UNKNOWN[]  = "UNKNOWN";
}


std::string to_string(Type typ) {
  switch(typ) {
    case Type::DEFAULT:
      return Encoder_DEFAULT;
    case Type::PLAIN:
      return Encoder_PLAIN;
    case Type::ZLIB:
      return Encoder_ZLIB;
    case Type::SNAPPY:
      return Encoder_SNAPPY;
    case Type::ZSTD:
      return Encoder_ZSTD;
    case Type::UNKNOWN:
      return Encoder_UNKNOWN;
    default:
      return std::string("UNKNOWN(" + std::to_string((uint8_t)typ) +")");
  }
}

Type encoding_from(const std::string& typ) {

  if(strncasecmp(typ.data(), Encoder_DEFAULT, typ.length()) == 0 || 
     typ.compare("0") == 0)
    return Type::DEFAULT;

  if(strncasecmp(typ.data(), Encoder_PLAIN, typ.length()) == 0 || 
     typ.compare("1") == 0)
    return Type::PLAIN;

  if(strncasecmp(typ.data(), Encoder_ZLIB, typ.length()) == 0 ||
     typ.compare("2") == 0)
    return Type::ZLIB;

  if(strncasecmp(typ.data(), Encoder_SNAPPY, typ.length()) == 0 ||
     typ.compare("3") == 0)
    return Type::SNAPPY;
  
  if(strncasecmp(typ.data(), Encoder_ZSTD, typ.length()) == 0 ||
     typ.compare("4") == 0)
    return Type::ZSTD;

  return Type::UNKNOWN;
}

SWC_SHOULD_INLINE
std::string repr_encoding(int typ) {
  return to_string((Type)typ);
}

SWC_SHOULD_INLINE
int from_string_encoding(const std::string& typ) {
  return (int)encoding_from(typ);
}



void decode(int& err, Type encoder, 
            const uint8_t* src, size_t sz_enc, 
            uint8_t *dst, size_t sz) {

  switch(encoder) {
    case Type::ZLIB: {
      z_stream strm;
      memset(&strm, 0, sizeof(z_stream));
      strm.zalloc = Z_NULL;
      strm.zfree = Z_NULL;
      strm.opaque = Z_NULL;
      strm.avail_in = 0;
      strm.next_in = Z_NULL;
      if(::inflateInit(&strm) != Z_OK) {
        err = Error::ENCODER_DECODE;
        return;
      }
      strm.avail_in = sz_enc;
      strm.next_in = (Bytef *)src;
      strm.avail_out = sz;
      strm.next_out = dst;
      if(::inflate(&strm, Z_NO_FLUSH) != Z_STREAM_END || strm.avail_out)
        err = Error::ENCODER_DECODE;
      ::inflateReset(&strm);
      return;
    }

    case Type::SNAPPY: {
      if(!snappy::RawUncompress((const char *)src, sz_enc, (char *)dst))
        err = Error::ENCODER_DECODE;
      return;
    }

    case Type::ZSTD: {
      if(ZSTD_decompress((void *)dst, sz, (void *)src, sz_enc) != sz)
        err = Error::ENCODER_DECODE;
      return;
    }

    default: {
      //SWC_ASSERT(encoder==Type::PLAIN);
      break;
    }
  }
}


void encode(int&, Type encoder, 
            const uint8_t* src, size_t src_sz, 
            size_t* sz_enc, DynamicBuffer& output, 
            uint32_t reserve) {
  
  switch(encoder) {
    case Type::ZLIB: {

      z_stream strm;
      memset(&strm, 0, sizeof(z_stream));
      strm.zalloc = Z_NULL;
      strm.zfree = Z_NULL;
      strm.opaque = Z_NULL;
      if(::deflateInit(&strm, Z_DEFAULT_COMPRESSION) == Z_OK) {

        uint32_t avail_out = src_sz + 6 + (((src_sz / 16000) + 1) * 5);
        output.ensure(reserve + avail_out);
        output.ptr += reserve;
        
        strm.avail_out = avail_out;
        strm.next_out = output.ptr;
        strm.avail_in = src_sz;
        strm.next_in = (Bytef*)src;
        if(::deflate(&strm, Z_FINISH) == Z_STREAM_END)
          *sz_enc = avail_out - strm.avail_out;
      }
      ::deflateReset(&strm);
      if(*sz_enc && *sz_enc < src_sz) {
        output.ptr += *sz_enc;
        return;
      }
      break;
    }

    case Type::SNAPPY: {
      output.ensure(reserve + snappy::MaxCompressedLength(src_sz));
      output.ptr += reserve;
      snappy::RawCompress((const char *)src, src_sz, 
                          (char *)output.ptr, sz_enc);
      if(*sz_enc && *sz_enc < src_sz) {
        output.ptr += *sz_enc;
        return;
      }
      break;
    }

    case Type::ZSTD: {
      size_t const avail_out = ZSTD_compressBound(src_sz);
      output.ensure(reserve + avail_out);
      output.ptr += reserve;
      
      *sz_enc = ZSTD_compress(
        (void *)output.ptr, avail_out,
        (void *)src, src_sz,
        ZSTD_CLEVEL_DEFAULT
      );
      if(*sz_enc && !ZSTD_isError(*sz_enc) && *sz_enc < src_sz) {
        output.ptr += *sz_enc;
        return;
      }
      break;
    }

    default: 
      break;
  }

  *sz_enc = 0; 
  output.free();

  output.ensure(reserve + src_sz);
  output.ptr += reserve;
  if(src_sz)
    output.add_unchecked(src, src_sz);
}


}}}
