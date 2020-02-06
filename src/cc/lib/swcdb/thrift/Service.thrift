/**
 * Copyright (C) 2019 SWC-DB (author: Kashirin Alex (kashirin.alex@gmail.com))
 */


/**
 * namespace for target languages
 */

namespace cpp   SWC.Thrift
namespace py    SWC.client.thrift
/*
namespace java  org.swcdb.thrift
namespace perl  SWC.thrift
namespace php   SWC
namespace rb    SWC.thrift
*/


exception Exception {
  1: i32 code
  2: string message
}

/* SCHEMAS */
enum ColumnMng {
  CREATE                = 3,
  DELETE                = 5,
  MODIFY                = 7
}

enum ColumnType {
  UNKNOWN       = 0,
  PLAIN         = 1,
  COUNTER_I64   = 2,
  COUNTER_I32   = 3,
  COUNTER_I16   = 4,
  COUNTER_I8    = 5,
  CELL_DEFINED  = 15
}

enum EncodingType {
  DEFAULT = 0,
  PLAIN   = 1,
  ZLIB    = 2,
  SNAPPY  = 3
}

struct Schema {
  1: optional i64           cid
  2: optional string 	  	  col_name
  3: optional ColumnType    col_type
  
  4: optional i32           cell_versions
  5: optional i32           cell_ttl
  
  6: optional i8            blk_replication
  7: optional EncodingType  blk_encoding
  8: optional i32           blk_size
  9: optional i32           blk_cells
  
  10: optional i32          cs_size
  11: optional i8           cs_max
  12: optional i8           compact_percent
  
  13: optional i64          revision
}
typedef list<Schema> Schemas



/* CELLS */
enum Flag {
  NONE            = 0,
  INSERT          = 1,
  DELETE          = 2,
  DELETE_VERSION  = 3
}

typedef list<binary> Key

struct Cell {
  1: string           c
  2: Key              k
  3: i64              ts
  4: optional binary  v
}
typedef list<Cell> Cells


struct CCell {
  1: Key              k
  2: i64              ts
  3: optional binary  v
}
typedef list<CCell> ColCells
typedef map<string, ColCells> CCells


struct KCell {
  1: string           c
  2: i64              ts
  3: optional binary  v
}
struct kCells {
  1: Key            k
  2: list<KCell>    cells
}
typedef list<kCells> KCells


struct FCell {
  1: string           c
  2: i64              ts
  3: optional binary  v
}
struct FCells {
  1: map<binary, FCells>   f
  2: optional list<FCell>  cells
}


service Service {

  void             sql_mng_column(1:string sql)   throws (1:Exception e),

  Schemas          sql_list_columns(1:string sql) throws (1:Exception e),


  Cells   sql_select(1:string sql)                   throws (1:Exception e),

  CCells  sql_select_rslt_on_column(1:string sql)    throws (1:Exception e),
  
  KCells  sql_select_rslt_on_key(1:string sql)       throws (1:Exception e),

  FCells  sql_select_rslt_on_fraction(1:string sql)  throws (1:Exception e),

}
