/*
 * Copyright (C) 2019 SWC-DB (author: Kashirin Alex (kashirin.alex@gmail.com))
 */


#include <dlfcn.h>
#include "swcdb/utils/Settings.h"

namespace SWC { namespace Utils { 


int run(const std::string& cmd, bool custom=false) {

  std::string lib_path(Env::Config::settings()->get<std::string>("lib-path"));
  if(custom) {
    lib_path.append(Env::Config::settings()->get<std::string>("lib"));
  } else {
    lib_path.append("libswcdb_utils_"); 
    lib_path.append(cmd);
    lib_path.append(".so");// {lib-path}/libswcdb_utils_shell.so
  }

  const char* err = dlerror();
  void* handle = dlopen(lib_path.c_str(), RTLD_NOW | RTLD_LAZY | RTLD_LOCAL);
  if (handle == NULL || err != NULL)
    HT_THROWF(Error::CONFIG_BAD_VALUE, 
              "Shared Lib %s, open fail: %s\n", lib_path.c_str(), err);

  err = dlerror();
  std::string handler_name =  "swc_utils_apply_cfg";
  void* f_cfg_ptr = dlsym(handle, handler_name.c_str());
  if (err != NULL || f_cfg_ptr == nullptr)
    HT_THROWF(Error::CONFIG_BAD_VALUE, 
              "Shared Lib %s, link(%s) fail: %s handle=%d\n", 
              lib_path.c_str(), handler_name.c_str(), err, (size_t)handle);
  ((swc_utils_apply_cfg_t*)f_cfg_ptr)(Env::Config::get());
    
  err = dlerror();
  handler_name =  "swc_utils_run";
  void* f_new_ptr = dlsym(handle, handler_name.c_str());
  if (err != NULL || f_new_ptr == nullptr)
    HT_THROWF(Error::CONFIG_BAD_VALUE, 
              "Shared Lib %s, link(%s) fail: %s handle=%d\n", 
              lib_path.c_str(), handler_name.c_str(), err, (size_t)handle);
  return ((swc_utils_run_t*)f_new_ptr)();
}



int not_implemented(const std::string& cmd) {
  SWC_LOGF(LOG_ERROR, "Not implemented, Utils command=%s", cmd.c_str());
  return 1;
}

}} // namespace SWC::Utils



int main(int argc, char** argv) {
  SWC::Env::Config::init(argc, argv);
  //std::cout << SWC::Env::Config::settings()->properties.to_string(true) << "\n";
  const auto& command = SWC::Env::Config::settings()->get<std::string>("command");
  
  if(command.compare("shell") == 0)
    return SWC::Utils::run(command);
  if(command.compare("custom") == 0)
    return SWC::Utils::run("command", true);
  
  return SWC::Utils::not_implemented(command);
}