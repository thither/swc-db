/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swcdb_core_LockAtomicUnique_h
#define swcdb_core_LockAtomicUnique_h

#include <atomic>
#include <thread>


namespace SWC { namespace LockAtomic {

class Unique {
  public:

  explicit Unique(): want(false) { }

  Unique(const Unique&) = delete;

  Unique(const Unique&&) = delete;
    
  Unique& operator=(const Unique&) = delete;

  ~Unique() { }

  bool try_lock() const {
    bool at=false;
    return want.compare_exchange_weak(at, true, std::memory_order_seq_cst);
  }
  
  void lock() const {
    uint16_t i = 0;
    for(auto at=false;
        !want.compare_exchange_weak(at, true, std::memory_order_seq_cst);
        at=false)
      if(++i == 0)
        std::this_thread::yield();
  }

  void lock(const uint32_t& us_sleep) const {
    uint16_t i = 0;
    for(auto at=false;
        !want.compare_exchange_weak(at, true, std::memory_order_seq_cst);
        at=false)
      if(++i == 0)
        std::this_thread::sleep_for(std::chrono::microseconds(us_sleep));
  }

  void unlock() const {
    want.store(false, std::memory_order_release);
  }
  
  class scope final {
    public:

    scope(const Unique& m) : _m(m) {  _m.lock(); }

    ~scope() { _m.unlock(); }
    
    scope(const scope&) = delete;

    scope(const scope&&) = delete;

    scope& operator=(const scope&) = delete;

    private:
    const Unique&      _m;
  };

  private:

  mutable std::atomic<bool> want;
};

}}

// SWC_LOCK_WITH_SUPPORT or use Mutex with Mutex::scope 
#define SWC_LOCK_WITH_SUPPORT(_mutex_, _state_, _code_, _return_) \
  if(_state_.try_lock()) { \
    _code_; \
    _state_.unlock(); \
  } else { \
    _mutex_.lock(); _state_.lock(); \
    _code_; \
    _state_.unlock(); _mutex_.unlock(); \
  } \
  _return_;

#endif // swcdb_core_LockAtomicUnique_h
