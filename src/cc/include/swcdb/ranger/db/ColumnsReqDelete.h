/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#ifndef swcdb_ranger_db_ColumnsReqDelete_h
#define swcdb_ranger_db_ColumnsReqDelete_h


namespace SWC { namespace Ranger {

struct ColumnsReqDelete {
  ColumnsReqDelete(const cid_t cid, const 
                   Comm::ConnHandlerPtr& conn, const Comm::Event::Ptr& ev) 
                  : cid(cid), conn(conn), ev(ev) {
  }
  ~ColumnsReqDelete() { }

  void cb(int err) {
    if(err)
      conn->send_error(err, "", ev);
    else
      conn->response_ok(ev);
  }

  const cid_t           cid;
  Comm::ConnHandlerPtr  conn;
  Comm::Event::Ptr      ev;
};

}} // namespace SWC::Ranger


#endif // swcdb_ranger_db_ColumnsReqDelete_h
