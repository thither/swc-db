
/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swcdb_db_types_MngrColumnState_h
#define swcdb_db_types_MngrColumnState_h

#include <string>

namespace SWC { namespace DB { namespace Types { 


namespace MngrColumn {

  enum State {
    NOTSET  = 0,
    OK      = 1,
    LOADING = 2,
    DELETED = 3
  };

}


std::string to_string(MngrColumn::State state);


}}}

#ifdef SWC_IMPL_SOURCE
#include "swcdb/db/Types/MngrColumnState.cc"
#endif 

#endif // swcdb_db_types_MngrColumnState_h
