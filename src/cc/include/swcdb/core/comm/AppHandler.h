/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#ifndef swcdb_core_comm_AppHandler_h
#define swcdb_core_comm_AppHandler_h

#include "swcdb/core/comm/Event.h"
#include "swcdb/core/comm/ConnHandler.h"

namespace SWC { namespace Comm {

typedef void (*AppHandler_t)(const ConnHandlerPtr& conn,
                             const Event::Ptr& ev);

class AppHandler {
  public:

  AppHandler(const ConnHandlerPtr& conn, const Event::Ptr& ev)
            : m_conn(conn), m_ev(ev) {
  }
    
  virtual ~AppHandler() { }

  virtual void run() = 0;

  protected:
  ConnHandlerPtr  m_conn;
  Event::Ptr      m_ev;
};


}} // namespace SWC::Comm



#endif // swcdb_core_comm_AppHandler_h
