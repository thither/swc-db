/*
 * Copyright Since 2019 SWC-DB© [author: Kashirin Alex kashirin.alex@gmail.com]
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */

#ifndef swc_utils_ShellFs_h
#define swc_utils_ShellFs_h

#include "swcdb/fs/Interface.h"


namespace SWC { namespace Utils { namespace shell {


class Fs : public Interface {
  public:
  Fs() 
    : Interface("\033[32mSWC-DB(\033[36mfs-" + 
                Env::Config::settings()->get_str("swc.fs") + 
                "\033[32m)\033[33m> \033[00m",
                "/tmp/.swc-cli-fs-" + 
                Env::Config::settings()->get_str("swc.fs") + "-history") {

    Env::FsInterface::init(FS::fs_type(
      Env::Config::settings()->get_str("swc.fs")));

    options.push_back(
      new Option(
        "list", 
        {"list directory contents",
         "list 'path';"},
        [ptr=this](const std::string& cmd){ return ptr->ls(cmd); }, 
        new re2::RE2("(?i)^(ls|list)")
      )
    );
  }

  virtual ~Fs() {
    Env::FsInterface::reset();
  }


  bool ls(const std::string& cmd) {

    std::string path;
    RE2::FullMatch(cmd, "(?i)^(ls|list)\\s+(.*)$", (void*)NULL, &path);
    if(path.front() == '"' || path.front() == '\'')
      path = path.substr(1, path.size()-2);

    SWC_PRINT << "Listing path='" << path << "':" << SWC_PRINT_CLOSE;
    FS::DirentList entries;
    Env::FsInterface::fs()->readdir(err, path, entries);
    if(err)
      return error("Problem reading '"+path+"'");
    
    size_t lname = 0;
    std::vector<FS::Dirent*> dirs;
    std::vector<FS::Dirent*> files;
    for(auto& entry : entries) {
      if(entry.name.size() > lname)
        lname = entry.name.size();

      auto& tmp = entry.is_dir ? dirs : files;
      for(auto it = tmp.begin(); ; ++it) {
        if(it == tmp.end() || 
           Condition::lt_volume(
             (const uint8_t*)(*it)->name.c_str(), (*it)->name.size(),
             (const uint8_t*)entry.name.c_str(), entry.name.size() ) ) {
          tmp.insert(it, &entry);
          break;
        }
      }
    }

    time_t t_secs;
    char modified[20];

    SWC_PRINT << "Directories=" << dirs.size() << ":\n";
    for(auto& entry : dirs) {
      t_secs = (time_t)entry->last_modification_time;
      std::strftime(modified, 20, "%Y/%m/%d %H:%M:%S", std::gmtime(&t_secs));
        
      std::cout << std::left << "  " 
                << std::left << std::setw(lname + 2) 
                << format("'%s'", entry->name.c_str())
                << std::left << "  modified=" << modified 
                << '\n';
    }

    std::cout << "Files=" << files.size() << ":\n";
    for(auto& entry : files) {
      t_secs = (time_t)entry->last_modification_time;
      std::strftime(modified, 20, "%Y/%m/%d %H:%M:%S", std::gmtime(&t_secs));
      
      std::cout << std::left << "  " 
                << std::left << std::setw(lname + 2) 
                << format("'%s'", entry->name.c_str())
                << std::left << "  modified=" << modified 
                << "  size=" << entry->length
                << '\n';
    }

    std::cout << SWC_PRINT_CLOSE;
    return true;
  }
};



}}} // namespace Utils::shell

#endif // swc_utils_ShellFs_h
