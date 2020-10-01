
/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swcdb_db_protocol_mngr_params_RgrGet_h
#define swcdb_db_protocol_mngr_params_RgrGet_h


#include "swcdb/db/Types/Identifiers.h"
#include "swcdb/db/Protocol/Common/params/HostEndPoints.h"
#include "swcdb/db/Cells/CellKey.h"


namespace SWC { namespace Protocol { namespace Mngr { namespace Params {


class RgrGetReq : public Serializable {
  public:

  RgrGetReq(cid_t cid=0, rid_t rid=0, bool next_range=false);

  virtual ~RgrGetReq();
  
  void print(std::ostream& out) const;

  cid_t          cid;
  rid_t          rid;
  DB::Cell::Key  range_begin, range_end;
  bool           next_range;
  //int            had_err;

  private:

  size_t internal_encoded_length() const;
    
  void internal_encode(uint8_t** bufp) const;
    
  void internal_decode(const uint8_t** bufp, size_t* remainp);

};



class RgrGetRsp : public Common::Params::HostEndPoints {
  public:

  RgrGetRsp(int err = Error::OK);

  RgrGetRsp(cid_t cid, rid_t rid);

  virtual ~RgrGetRsp();

  int             err;         
  cid_t           cid; 
  rid_t           rid; 
  DB::Cell::Key   range_end;
  DB::Cell::Key   range_begin;

  void print(std::ostream& out) const;

  private:

  size_t internal_encoded_length() const;
    
  void internal_encode(uint8_t** bufp) const;
    
  void internal_decode(const uint8_t** bufp, size_t* remainp);

};
  

}}}}



#ifdef SWC_IMPL_SOURCE
#include "swcdb/db/Protocol/Mngr/params/RgrGet.cc"
#endif 

#endif // swcdb_db_protocol_mngr_params_RgrGet_h
