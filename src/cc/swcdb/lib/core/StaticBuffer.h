/*
 * Copyright (C) 2019 SWC-DB (author: Kashirin Alex (kashirin.alex@gmail.com))
 * Copyright (C) 2007-2016 Hypertable, Inc.
 *
 * This file is part of Hypertable.
 *
 * Hypertable is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or any later version.
 *
 * Hypertable is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */


#ifndef swc_core_StaticBuffer_h
#define swc_core_StaticBuffer_h

#include "Logger.h"
#include "DynamicBuffer.h"

#include <cstdlib>
#include <memory>

namespace SWC {

  /** A memory buffer of static size. The actual buffer can be allocated or
   * assigned by the caller. If the StaticBuffer "owns" the pointer then it
   * will be released when going out of scope.
   */
  class StaticBuffer {
  public:
  
    typedef std::shared_ptr<StaticBuffer> Ptr;

    /** Constructor.  Creates an empty buffer */
    StaticBuffer() : base(0), size(0), own(true) {
    }

    /** Constructor.
     * Allocates a new buffer of size <code>len</code>. If
     *
     * @param len The size of the new buffer, in bytes
     */
    explicit StaticBuffer(size_t len) : size(len), own(true) {
      allocate();
    }

    void allocate(){
      base = new uint8_t[size];
    }

    void reallocate(uint32_t len) {
      free();
      own = true;
      size = len;
      allocate();
    }

    /** Constructor; assigns an existing buffer and can take ownership of
     * that buffer.
     *
     * @param data Pointer to the existing buffer
     * @param len Size of the existing buffer
     * @param take_ownership If yes, will "own" the existing buffer and
     *      delete[] the memory when going out of scope. Make sure that the
     *      buffer was allocated with new[]!
     */
    StaticBuffer(void *data, uint32_t len, bool take_ownership = true)
                : base((uint8_t *)data), size(len), own(take_ownership) {
    }

    /** Constructor; takes ownership from a DynamicBuffer */
    StaticBuffer(DynamicBuffer &dbuf) {
      base = dbuf.base;
      size = dbuf.fill();
      own = dbuf.own;
      if (own) {
        dbuf.base = dbuf.ptr = 0;
        dbuf.size = 0;
      }
    }

    /**
     * Copy constructor.
     *
     * WARNING: This assignment operator will cause the ownership of the buffer
     * to transfer to the lvalue buffer if the own flag is set to 'true' in the
     * buffer being copied.  The buffer being copied will be modified to have
     * it's 'own' flag set to false and the 'base' pointer will be set to NULL.
     * In other words, the buffer being copied is no longer usable after the
     * assignment.
     *
     * @param other Reference to the original instance
     */
    StaticBuffer(StaticBuffer& other) {
      base = other.base;
      size = other.size;
      own = other.own;
      if (own) {
        other.own = false;
        other.base = 0;
      }
    }

    void set(StaticBuffer& other) {
      free();
      base = other.base;
      size = other.size;
      own = other.own;
      if (own) {
        other.own = false;
        other.base = 0;
        other.size = 0;
      }
    }

    void set(DynamicBuffer &dbuf) {
      free();
      base = dbuf.base;
      size = dbuf.fill();
      own = dbuf.own;
      if (own) {
        dbuf.base = dbuf.ptr = 0;
        dbuf.size = 0;
      }
    }

    /** Sets data pointer; the existing buffer is discarded and deleted
     *
     * @param data Pointer to the existing buffer
     * @param len Size of the existing buffer
     * @param take_ownership If yes, will "own" the existing buffer and
     *      delete[] the memory when going out of scope. Make sure that the
     *      buffer was allocated with new[]!
     */
    void set(uint8_t *data, uint32_t len, bool take_ownership = true) {
      free();
      base = data;
      size = len;
      own = take_ownership;
    }

    /** Clears the data; if this object is owner of the data then the allocated
     * buffer is delete[]d */
    void free() {
      if (own && base)
        delete [] base;
      base = 0;
      size = 0;
    }

    /** Destructor; if "own" is true then the buffer will be delete[]d */
    ~StaticBuffer() {
      free();
    }

    uint8_t   *base;
    uint32_t  size;
    bool      own;

  };

  /** "Less than" operator for StaticBuffer; uses @a memcmp */
  inline bool operator<(const StaticBuffer& sb1, const StaticBuffer& sb2) {
    size_t len = (sb1.size < sb2.size) ? sb1.size : sb2.size;
    int cmp = memcmp(sb1.base, sb2.base, len);
    return (cmp==0) ? sb1.size < sb2.size : cmp < 0;
  }

  /** Equality operator for StaticBuffer; uses @a memcmp */
  inline bool operator==(const StaticBuffer& sb1, const StaticBuffer& sb2) {
    if (sb1.size != sb2.size)
      return false;
    size_t len = (sb1.size < sb2.size) ? sb1.size : sb2.size;
    int equal = memcmp(sb1.base, sb2.base, len);
    if (equal == 0 )
      return true;
    return false;
  }

  /** Inequality operator for StaticBuffer */
  inline bool operator!=(const StaticBuffer& sb1, const StaticBuffer& sb2) {
    return !(sb1 == sb2);
  }


} // namespace SWC

#endif // swc_core_StaticBuffer_h
