/*
 * Copyright (C) 2019 SWC-DB (author: Kashirin Alex (kashirin.alex@gmail.com))
 */


#ifndef swcdb_db_Files_BlockLoader_h
#define swcdb_db_Files_BlockLoader_h

#include "swcdb/db/Files/RangeBlock.h"
#include "swcdb/db/Files/CellStoreBlock.h"
#include "swcdb/db/Files/CommitLogFragment.h"

namespace SWC { namespace Files { namespace Range {


class BlockLoader final {
  public:

  int64_t               ts;
  Range::Block::Ptr     block;

  BlockLoader(Range::Block::Ptr block);

  ~BlockLoader();

  void run();

  void add(CellStore::Block::Read::Ptr blk);

  void add(CommitLog::Fragment::Ptr frag);

  void loaded_blk();

  void loaded_frag();

  private:

  void load_cellstores();

  void load_log();

  void loaded_cellstores();

  void loaded_log();

  void completion();
  

  std::mutex                                m_mutex;
  int                                       m_err;
  bool                                      m_processing;
  std::vector<CellStore::Block::Read::Ptr>  m_cs_blocks;
  std::vector<CommitLog::Fragment::Ptr>     m_fragments;
  bool                                      m_chk_cs;
  bool                                      m_chk_log;
};



}}}

#endif