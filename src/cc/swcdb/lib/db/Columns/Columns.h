/*
 * Copyright (C) 2019 SWC-DB (author: Kashirin Alex (kashirin.alex@gmail.com))
 */


#ifndef swcdb_lib_db_Columns_Columns_h
#define swcdb_lib_db_Columns_Columns_h

#include "swcdb/lib/fs/Interface.h"
#include "swcdb/lib/db/Types/RsRole.h"

#include "swcdb/lib/core/comm/ResponseCallback.h"
#include "Column.h"

#include <mutex>
#include <memory>
#include <unordered_map>
#include <iostream>

namespace SWC {

typedef std::unordered_map<int64_t, ColumnPtr> ColumnsMap;
typedef std::pair<int64_t, ColumnPtr> ColumnsMapPair;



class Columns : public std::enable_shared_from_this<Columns> {

  public:

  Columns() : columns(std::make_shared<ColumnsMap>()) {}

  virtual ~Columns(){}

  ColumnPtr get_column(int64_t cid, bool load){
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = columns->find(cid);
    if (it != columns->end())
      return it->second;

    if(load) {
      ColumnPtr col = std::make_shared<Column>(cid);
      if(col->has_err() == 0)
        columns->insert(ColumnsMapPair(cid, col));
      return col;
    }
    return nullptr;
  }

  RangePtr get_range(int64_t cid, int64_t rid, 
                     bool initialize=false, bool load=false, bool is_rs=false){
    ColumnPtr col = get_column(cid, load);
    if(col == nullptr) 
      return nullptr;
    return col->get_range(rid, initialize, is_rs);
  }

  void load_range(int64_t cid, int64_t rid, ResponseCallbackPtr cb){
    RangePtr range = get_range(cid, rid, true, true, true); // only RS
    if(range != nullptr && range->is_loaded()) {
      cb->response_ok();
      return;
    }
    
    cb->send_error(Error::NOT_LOADED_RANGE , "");
  }

  private:
  std::mutex                  m_mutex;
  std::shared_ptr<ColumnsMap> columns;

};
typedef std::shared_ptr<Columns> ColumnsPtr;



class EnvColumns {
  public:

  static void init() {
    m_env = std::make_shared<EnvColumns>();
  }

  static ColumnsPtr get(){
    HT_ASSERT(m_env != nullptr);
    return m_env->m_columns;
  }

  EnvColumns() : m_columns(std::make_shared<Columns>()) {}
  virtual ~EnvColumns(){}

  private:
  ColumnsPtr                  m_columns = nullptr;
  inline static std::shared_ptr<EnvColumns> m_env = nullptr;
};


}
#endif