/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swcdb_manager_ClientContextManager_h
#define swcdb_manager_ClientContextManager_h

namespace SWC { namespace client { 


//! The SWC-DB Manager's Client to Database C++ namespace 'SWC::client::Mngr'
namespace Mngr { 


class ContextManager final : public client::ContextManager {
  public:

  ContextManager();

  virtual ~ContextManager();

  void disconnected(const Comm::ConnHandlerPtr& conn);

  void handle(Comm::ConnHandlerPtr conn, const Comm::Event::Ptr& ev) override;

};

}}}

#endif // swcdb_manager_ClientContextManager_h