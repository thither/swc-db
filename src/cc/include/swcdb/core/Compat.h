/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swcdb_core_Compact_h
#define swcdb_core_Compact_h



//! The SWC-DB C++ namespace 'SWC'
namespace SWC {


/**
 * \defgroup Core The Core-Components Group
 * @brief A group with all related to SWC-DB Core-Components (libswcdb_core, libswcdb_core_config, libswcdb_core_comm).
 *
 *
 */

/**
 * @brief The SWC-DB Core Components C++ namespace 'SWC::Core'
 *
 * \ingroup Core
 */
namespace Core {


static_assert(
  0xff >> 1 == 0x7f,
  "SWC-DB supports only Little-Endian Architecture!"
);

static_assert(
  0x01lu << 32 > 0xffffffff,
  "SWC-DB supports only 64+ bits Architecture!"
);

static_assert(
  sizeof(long double) == 16,
  "SWC-DB supports only long double of 128 bits Architecture!"
);


}

}


/*!
 *  \addtogroup Core
 *  @{
 */

#define SWC_STRINGIFY(s) #s

#define SWC_ATTRIBS(attrs) __attribute__(attrs)
#define SWC_ATTR_NOTHROW __nothrow__
#define SWC_ATTR_INLINE __always_inline__ //, __artificial__

#define SWC_NOEXCEPT noexcept(true)


#ifdef SWC_IMPL_SOURCE
# define SWC_SHOULD_INLINE  \
  SWC_ATTRIBS((SWC_ATTR_INLINE)) \
  extern inline
#else
# define SWC_SHOULD_INLINE
#endif



#if defined(__clang__) // CLANG specific

  #define SWC_SHOULD_NOT_INLINE  \
    SWC_ATTRIBS((__noinline__))

  #define SWC_ATTRIB_O3

  #define SWC_PRAGMA_DIAGNOSTIC_PUSH _Pragma("clang diagnostic push")
  #define SWC_PRAGMA_DIAGNOSTIC_IGNORED(_flags_) \
    _Pragma(SWC_STRINGIFY(clang diagnostic ignored _flags_))
  #define SWC_PRAGMA_DIAGNOSTIC_POP  _Pragma("clang diagnostic pop")


#elif defined(__GNUC__) // GCC specific

  #define SWC_SHOULD_NOT_INLINE  \
    SWC_ATTRIBS((__noinline__, __noclone__))

  #define SWC_ATTRIB_O3  \
    optimize("-O3")

  #define SWC_PRAGMA_DIAGNOSTIC_PUSH _Pragma("GCC diagnostic push")
  #define SWC_PRAGMA_DIAGNOSTIC_IGNORED(_flags_) \
    _Pragma(SWC_STRINGIFY(GCC diagnostic ignored _flags_))
  #define SWC_PRAGMA_DIAGNOSTIC_POP  _Pragma("GCC diagnostic pop")

#endif



# define SWC_CAN_INLINE  \
  SWC_ATTRIBS((SWC_ATTR_INLINE)) \
  inline


/*! @} End of Core Group*/





#include <stddef.h>



#if defined MIMALLOC
#include <mimalloc.h> // -override
#endif


#if !defined(SWC_MALLOC_NOT_INSISTENT) && !defined(__clang__)
#include "swcdb/core/Malloc.h"

#elif defined MIMALLOC
#include <mimalloc-new-delete.h>
#endif




#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <cstddef> // for std::size_t and std::ptrdiff_t
#include <memory>
#include <cstring>
#include <string>
#include <vector>
#include <functional>
#include <thread>




/*!
 *  \addtogroup Core
 *  @{
 */



#ifdef SWC_USE_ABORT
  #define SWC_ABORT abort()
#else
  #define SWC_ABORT raise(SIGABRT)
#endif


#if defined(SWC_ENABLE_SANITIZER)
  #define SWC_CAN_QUICK_EXIT(_CODE_)
#else
  #define SWC_CAN_QUICK_EXIT(_CODE_) std::quick_exit(_CODE_)
#endif



/*! @} End of Core Group*/



#include "swcdb/core/Atomic.h"
#include "swcdb/core/BitFieldInt.h" // for u/int24/40/48/56_t
#include "swcdb/core/Comparators_basic.h"
#include "swcdb/core/MutexLock.h"

#endif // swcdb_core_Compact_h
