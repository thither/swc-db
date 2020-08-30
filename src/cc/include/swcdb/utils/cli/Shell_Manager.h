/*
 * Copyright Since 2019 SWC-DB© [author: Kashirin Alex kashirin.alex@gmail.com]
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swc_utils_ShellManager_h
#define swc_utils_ShellManager_h

#include "swcdb/utils/cli/Shell.h"


namespace SWC { namespace Utils { namespace shell {


class Mngr : public Interface {
  public:

  Mngr();

  bool cluster_status(std::string&);

  bool managers_status(std::string& cmd);

  bool column_status(std::string& cmd);

  bool rangers_status(std::string& cmd);

};



}}} // namespace Utils::shell

#endif // swc_utils_ShellManager_h
