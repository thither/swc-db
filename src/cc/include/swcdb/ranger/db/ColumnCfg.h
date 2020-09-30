/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
*/

#ifndef swc_ranger_db_ColumnCfg_h
#define swc_ranger_db_ColumnCfg_h

#include "swcdb/db/Cells/KeyComparator.h"
#include "swcdb/db/Columns/Schema.h"

namespace SWC { namespace Ranger {


class ColumnCfg final {
  
  public:
  
  const cid_t                           cid;
  const Types::KeySeq                   key_seq;

  mutable std::atomic<Types::Column>    col_type;

  mutable std::atomic<uint32_t>         c_versions; 
  mutable std::atomic<uint64_t>         c_ttl;

  mutable std::atomic<Encoder::Type>    blk_enc;
  mutable std::atomic<uint32_t>         blk_size;
  mutable std::atomic<uint32_t>         blk_cells;

  mutable std::atomic<uint8_t>          cs_replication;
  mutable std::atomic<uint32_t>         cs_size;
  mutable std::atomic<uint8_t>          cs_max;
  mutable std::atomic<uint8_t>          log_rout_ratio;
  mutable std::atomic<uint8_t>          compact_perc;

  mutable std::atomic<bool>             deleting;


  ColumnCfg(const cid_t cid, const DB::Schema& schema) 
            : cid(cid), key_seq(schema.col_seq),
              deleting(false) {
    update(schema);
  }

  ColumnCfg(const ColumnCfg&) = delete;

  ColumnCfg(const ColumnCfg&&) = delete;
  
  ColumnCfg& operator=(const ColumnCfg&) = delete;

  ~ColumnCfg() { }

  void update(const DB::Schema& schema) const {
    col_type = schema.col_type;

    c_versions = schema.cell_versions;
    c_ttl = schema.cell_ttl*1000000000;

    blk_enc = schema.blk_encoding;
    blk_size = schema.blk_size;
    blk_cells = schema.blk_cells;
    
    cs_replication = schema.cs_replication;
    cs_size = schema.cs_size;
    cs_max = schema.cs_max;
    log_rout_ratio = schema.log_rollout_ratio;
    compact_perc = schema.compact_percent;
  }

  Types::Column column_type() const {
    return col_type.load();
  }

  uint32_t cell_versions() const {
    return c_versions.load();
  }

  uint64_t cell_ttl() const {
    return c_ttl.load();
  }


  Encoder::Type block_enc() const {
    return blk_enc != Encoder::Type::DEFAULT
            ?  blk_enc.load() 
            : (Encoder::Type)Env::Rgr::get()->cfg_blk_enc->get();
  }

  uint32_t block_size() const {
    return blk_size 
            ? blk_size.load() 
            : Env::Rgr::get()->cfg_blk_size->get();
  }

  uint32_t block_cells() const {
    return blk_cells 
            ? blk_cells.load() 
            : Env::Rgr::get()->cfg_blk_cells->get();
  }


  uint8_t file_replication() const {
    return cs_replication 
            ? cs_replication.load() 
            : Env::Rgr::get()->cfg_cs_replication->get();
  }

  uint32_t cellstore_size() const {
    return cs_size 
            ? cs_size.load() 
            : Env::Rgr::get()->cfg_cs_sz->get();
  }

  uint8_t cellstore_max() const {
    return cs_max 
            ? cs_max.load() 
            : Env::Rgr::get()->cfg_cs_max->get();
  }

  uint8_t log_rollout_ratio() const {
    return log_rout_ratio
            ? log_rout_ratio.load() 
            : Env::Rgr::get()->cfg_log_rollout_ratio->get();
  }

  uint8_t compact_percent() const {
    return compact_perc 
            ? compact_perc.load() 
            : Env::Rgr::get()->cfg_compact_percent->get();
  }

  void print(std::ostream& out) const {
    out << "col(";
    if(deleting) 
      out << "DELETING ";
    out
      << "cid="   << cid
      << " seq="  << Types::to_string(key_seq)
      << " type=" << Types::to_string(col_type)
      << ')'
      << " cell(versions=" << c_versions
      << " ttl=" << c_ttl
      << ')'
      << " blk(enc="  << Encoder::to_string(blk_enc)
      << " size="     << blk_size
      << " cells="    << blk_cells
      << ')'
      << " cs(replication=" << (int)cs_replication
      << " size="           << cs_size
      << " max="            << (int)cs_max
      << " rollout="        << (int)log_rout_ratio
      << " compact="        << (int)compact_perc << '%'
      << ')';
  }

  friend std::ostream& operator<<(std::ostream& out, const ColumnCfg& cfg) {
    cfg.print(out);
    return out;
  }

};


}}

#endif // swc_ranger_db_ColumnCfg_h