/*
 * Copyright Since 2019 SWC-DB© [author: Kashirin Alex kashirin.alex@gmail.com]
 */

#ifndef swc_fs_SmartFd_h
#define swc_fs_SmartFd_h

#include "swcdb/core/Compat.h"
#include "swcdb/core/LockAtomicUnique.h"

namespace SWC{ namespace FS {

/// Smart FileDescriptor 

struct SmartFd {
  public:

  typedef std::shared_ptr<SmartFd> Ptr;
  
  static Ptr make_ptr(const std::string &filepath, uint32_t flags, 
                      int32_t fd=-1, uint64_t pos=0);

  SmartFd(const std::string &filepath, uint32_t flags, 
          int32_t fd=-1, uint64_t pos=0);

  virtual ~SmartFd();

  const std::string& filepath() const;

  void flags(uint32_t flags);

  uint32_t flags() const;

  void fd(int32_t fd);

  int32_t fd() const;

  void pos(uint64_t pos);
  
  uint64_t pos() const;

  bool valid() const;

  std::string to_string() const;

  protected:

  const std::string   m_filepath;
  LockAtomic::Unique  m_mutex;

  private:

  uint32_t            m_flags;
  int32_t             m_fd;
  uint64_t            m_pos;
};

}}



#ifdef SWC_IMPL_SOURCE
#include "swcdb/fs/SmartFd.cc"
#endif 

#endif  // swc_fs_SmartFd_h