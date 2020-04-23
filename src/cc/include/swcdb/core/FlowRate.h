/*
 * Copyright Since 2019 SWC-DB© [author: Kashirin Alex kashirin.alex@gmail.com]
 */

#ifndef swc_core_FlowRate_h
#define swc_core_FlowRate_h

#include <atomic>
#include <mutex>
#include "swcdb/core/LockAtomicUnique.h"


namespace SWC { namespace FlowRate {

static const size_t KB = 1024;
static const size_t MB = KB * 1024;
static const size_t GB = MB * 1024;
static const size_t TB = GB * 1024;
static const size_t PB = TB * 1024;

struct Data {
  double      bytes = 0;
  const char* bytes_base = "";
  double      time = 0;
  const char* time_base = "";
  
  Data(size_t in_bytes, size_t in_ns, 
       size_t epochs_bytes = 0, size_t epochs_ns = 0) {
    
    if(in_ns < 100000) {
      time = in_ns;
      time_base = "ns";
    } else if(in_ns < 10000000) { 
      time = (double)in_ns/1000;
      time_base = "us";
    } else if(in_ns <= 10000000000) { 
      time = (double)(in_ns/1000)/1000;
      time_base = "ms";
    } else if(in_ns <= 3600000000000) {
      time = (double)(in_ns/1000000)/1000;
      time_base = "s";
    } else if(in_ns <= 86400000000000) {
      time = (double)(in_ns/60000000)/1000;
      time_base = "M";
    } else {
      time = (double)(in_ns/3600000000)/1000;
      time_base = "H";
    }

    if(in_bytes <= MB) {
      bytes = in_bytes;
      bytes_base = "B";
    } else if(in_bytes <= GB ) {
      bytes = (double)in_bytes/KB;
      bytes_base = "KB";
    } else if(in_bytes <= TB) {
      bytes = (double)(in_bytes/KB)/KB;
      bytes_base = "MB";
    } else if(in_bytes <= PB) {
      bytes = (double)(in_bytes/MB)/KB;
      bytes_base = "GB";
    } else {
      bytes = (double)(in_bytes/GB)/KB;
      bytes_base = "TB";
    }

  }

  void print_cells_statistics(std::ostream& out, size_t cells_count, 
                              bool resend_cells=false) {
    out << "\n\nStatistics:\n"
        << " Total Time Took:        " << time << " " << time_base     << "\n"
        << " Total Cells Count:      " << cells_count                  << "\n"
        << " Total Cells Size:       " << bytes << " " << bytes_base   << "\n";
        
    if(resend_cells)
    out << " Resend Cells Count:     " << resend_cells                 << "\n";
    
    out << " Average Transfer Rate:  " << bytes/time << " " 
                                       << bytes_base <<"/"<< time_base << "\n"
        << " Average Cells Rate:     " << (cells_count ? cells_count/time : 0)
                                       << " cell/" << time_base
        << std::flush;
  }
};


} }

#endif // swc_core_FlowRate_h
