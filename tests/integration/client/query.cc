/*
 * Copyright Since 2019 SWC-DB© [author: Kashirin Alex kashirin.alex@gmail.com]
 */
 
#include "swcdb/core/config/Settings.h"
#include "swcdb/core/comm/Settings.h"

#include "swcdb/db/client/Clients.h"
#include "swcdb/db/client/AppContext.h"
#include "swcdb/db/Protocol/Mngr/req/ColumnMng.h"
#include "swcdb/db/Protocol/Mngr/req/ColumnGet.h"
#include "swcdb/db/client/Query/Select.h"



namespace SWC { 
  
  
namespace Config {

void Settings::init_app_options(){
  init_comm_options();
  init_client_options();

  cmdline_desc.add_options()
    ("ncells", i64(1000), "number of cells, total=cells*(counter||versions)")
    ("nfractions", i32(26), "Number of Fractions per cell key") 
    
    ("value-size", i32(256), "value in bytes or counts for a counter")
    
    ("col-seq", 
      g_enum(
        (int)Types::KeySeq::LEXIC,
        0,
        Types::from_string_range_seq,
        Types::repr_range_seq
      ), 
     "Schema col-seq FC_+/LEXIC/VOLUME")  

    ("col-type", 
      g_enum(
        (int)Types::Column::PLAIN,
        0,
        Types::from_string_col_type,
        Types::repr_col_type
      ), 
     "Schema col-type PLAIN/COUNTER_I64/COUNTER_I32/COUNTER_I16/COUNTER_I8")  
    
    ("cell-versions", i32(1), "cell key versions")
  ;
}
void Settings::init_post_cmd_args(){ }

}


class Test {
  public:

  std::mutex                mutex;
  std::condition_variable   cv;

  Types::Column             col_type;
  Types::KeySeq             col_seq;
  std::string               col_name;
  uint32_t                  cell_versions;
  
  size_t                    ncells;
  uint32_t                  nfractions;

  bool                      runs;
  DB::Schema::Ptr           schema;
  bool                      counter;

  size_t                    time_select;
  void run() {
    counter = Types::is_counter(col_type);
    time_select = 0;

    SWC_PRINT << "Test::run "
              << " col_type=" << Types::to_string(col_type)
              << " col_seq=" << Types::to_string(col_seq)
              << " col_name=" << col_name
              << " cell_versions=" << cell_versions
              << " ncells=" << ncells
              << " nfractions=" << nfractions
              << SWC_PRINT_CLOSE;

    runs = true;
    create_column();

    std::unique_lock lock_wait(mutex);
    cv.wait(lock_wait, [this]{ return !runs; });
  }

  void create_column() {
    SWC_LOG(LOG_DEBUG, "create_column");

    int err = Error::OK;
    if(schema = Env::Clients::get()->schemas->get(err, col_name))
      return delete_column([this](){ create_column(); });

    schema = DB::Schema::make();
    schema->col_type = col_type;
    schema->col_name = col_name;
    schema->col_seq = col_seq;
    schema->cs_size = 200000000;
    schema->blk_cells = 10000;
    schema->cell_versions = counter ? 1 : cell_versions;

    Protocol::Mngr::Req::ColumnMng::request(
      Protocol::Mngr::Req::ColumnMng::Func::CREATE,
      schema, [this] (client::ConnQueue::ReqBase::Ptr req_ptr, int err) {
        if(err && err != Error::COLUMN_SCHEMA_NAME_EXISTS) {
          SWC_PRINT << "ColumnMng::CREATE err=" 
                    << err << "(" << Error::get_text(err) << ")"
                    << SWC_PRINT_CLOSE;
          return req_ptr->request_again();
        }
        schema = Env::Clients::get()->schemas->get(err, col_name);
        query_insert();
      },
      10000
    );
  }

  void delete_column(const std::function<void()>& cb) {
    SWC_LOG(LOG_DEBUG, "delete_column");

    Protocol::Mngr::Req::ColumnMng::request(
      Protocol::Mngr::Req::ColumnMng::Func::DELETE,
      schema, [this, cb] (client::ConnQueue::ReqBase::Ptr req_ptr, int err) {
        if(err && err != Error::COLUMN_SCHEMA_NAME_NOT_EXISTS) {
          SWC_PRINT << "ColumnMng::DELETE err=" 
                    << err << "(" << Error::get_text(err) << ")"
                    << SWC_PRINT_CLOSE;
          return req_ptr->request_again();
        }

        Env::Clients::get()->schemas->remove(schema->cid);
        schema = nullptr;
        cb();
      },
      10000
    );
  }
  

  void expect_empty_column() {
    SWC_LOG(LOG_DEBUG, "expect_empty_column");

    auto req = std::make_shared<client::Query::Select>();
  
    auto intval = DB::Specs::Interval::make_ptr();
    intval->flags.offset = 0;
    intval->flags.limit = 1;
    req->specs.columns = {
      DB::Specs::Column::make_ptr(schema->cid, {intval})
    };
  
    int err = Error::OK;
    req->scan(err);
    if(err) {
      SWC_PRINT << "expect_empty_column err=" 
                << err << "(" << Error::get_text(err) << ")"
                << SWC_PRINT_CLOSE;
    }
    SWC_ASSERT(!err);

    req->wait();
    SWC_ASSERT(!req->result->get_size(schema->cid));

    SWC_PRINT << "expect_empty_column:  \n";
    req->result->profile.print(std::cout); 
    std::cout << SWC_PRINT_CLOSE;
  }
 
  void expect_one_at_offset() {
    SWC_LOG(LOG_DEBUG, "expect_one_at_offset");

    auto req = std::make_shared<client::Query::Select>();
  
    auto intval = DB::Specs::Interval::make_ptr();
    intval->flags.offset = ncells * nfractions - 1;
    intval->flags.max_versions = 1;
    req->specs.columns = {
      DB::Specs::Column::make_ptr(schema->cid, {intval})
    };
  
    int err = Error::OK;
    req->scan(err);
    SWC_ASSERT(!err);

    req->wait();
    if(req->result->get_size(schema->cid) != 1) {
      SWC_PRINT << "get_size: " << req->result->get_size(schema->cid) << SWC_PRINT_CLOSE;
      SWC_ASSERT(req->result->get_size(schema->cid) == 1);
    }

    SWC_PRINT << "expect_one_at_offset:  \n";
    req->result->profile.print(std::cout); 
    std::cout << SWC_PRINT_CLOSE;
  }


  void apply_cell_key(DB::Cell::Key& key, uint32_t i, uint32_t f) {
    key.free();
    std::string cell_number(std::to_string(i));
    for(uint32_t chr=0; chr<=f; ++chr)
      key.add(((char)(uint8_t)(chr+97))+cell_number);
  }

  std::string apply_cell_value(const DB::Cell::Key& key) {
    std::string value = "V_OF:(" + key.to_string() + "):(";
    for(uint32_t chr=0; chr<=255; ++chr)
      value += (char)chr;
    value += ")END";
    return value;
  }



  void query_insert() {
    SWC_LOG(LOG_DEBUG, "query_insert");
    
    expect_empty_column();

    auto req = std::make_shared<client::Query::Update>(
      [this](const client::Query::Update::Result::Ptr& result) {
        SWC_PRINT << "query_insert: \n";
        result->profile.print(std::cout); 
        std::cout << SWC_PRINT_CLOSE;

        SWC_ASSERT(!result->error());
        expect_one_at_offset();
        query_select(0, 0);
      }
    );
    req->columns->create(schema);
    auto col = req->columns->get_col(schema->cid);

    DB::Cells::Cell cell;
    cell.flag = DB::Cells::INSERT;
    cell.set_time_order_desc(true);

    for(uint32_t i=0; i<ncells; ++i) {
      for(uint32_t v=0; v<cell_versions; ++v) {
        for(uint32_t f=0; f<nfractions; ++f) {
          apply_cell_key(cell.key, i, f);

          if(counter) {
            cell.set_counter(0, 1);
          } else {
            cell.set_value(
              apply_cell_value(cell.key)
            );
          }
          col->add(cell);
          req->commit_or_wait(col);
        }
      }
    }
    req->commit_if_need();
  }


  void query_select(uint32_t i, uint32_t f) {
    SWC_LOG(LOG_DEBUG, "query_select");
    
    if(f == nfractions) {
      f = 0;
      ++i;
    }
    if(i == ncells && !f) {
        SWC_PRINT << "query_select: \n"
                  << " time_select=" << time_select << "ns"
                  << " cells=" << (ncells*nfractions)
                  << " avg=" << time_select/(ncells*nfractions) << "ns\n"
                  << SWC_PRINT_CLOSE;
      return query_delete();
    }

    DB::Cell::Key key;
    apply_cell_key(key, i, f);

    auto req = std::make_shared<client::Query::Select>(
      [this, ts=Time::now_ns(), key=DB::Cell::Key(key), i, f]
      (const client::Query::Select::Result::Ptr& result) {
        time_select += Time::now_ns() - ts;
        SWC_LOG_OUT(LOG_DEBUG) << "query_select:  \n";
        result->profile.print(std::cout); 
        std::cout << SWC_LOG_OUT_END;
        
        SWC_ASSERT(!result->err);
        SWC_ASSERT(result->get_size(schema->cid) == counter?1:cell_versions);

        DB::Cells::Result cells;
        result->get_cells(schema->cid, cells);

        for(auto cell : cells) {
          SWC_ASSERT(key.equal(cell->key));
          if(counter)
            SWC_ASSERT(cell->get_counter() == cell_versions);
        }

        query_select(i, f + 1);
      }
    );
  
    auto intval = DB::Specs::Interval::make_ptr();
    intval->key_start.set(key, Condition::EQ);
    intval->flags.offset = 0;
    intval->flags.limit = counter ? 1 : cell_versions;
    req->specs.columns = {
      DB::Specs::Column::make_ptr(schema->cid, {intval})
    };
    SWC_LOG(LOG_DEBUG, intval->to_string());

    int err = Error::OK;
    req->scan(err);
    SWC_ASSERT(!err);
  }


  void query_delete() {
    SWC_LOG(LOG_DEBUG, "query_delete");
    
    auto req = std::make_shared<client::Query::Update>(
      [this](const client::Query::Update::Result::Ptr& result) {
        SWC_PRINT << "query_delete: \n";
        result->profile.print(std::cout); 
        std::cout << SWC_PRINT_CLOSE;

        SWC_ASSERT(!result->error());
        expect_empty_column();

        delete_column([this]() { 
          {
            std::unique_lock lock(mutex);
            runs = false;
          }
          cv.notify_all(); 
        });
      }
    );
    req->columns->create(schema);
    auto col = req->columns->get_col(schema->cid);

    DB::Cells::Cell cell;
    cell.flag = DB::Cells::DELETE;
    cell.set_time_order_desc(true);

    for(uint32_t i=0; i<ncells; ++i) {
      for(uint32_t f=0; f<nfractions; ++f) {
        apply_cell_key(cell.key, i, f);
        col->add(cell);
        req->commit_or_wait(col);
      }
    }
    req->commit_if_need();
  }
};


}

int main(int argc, char** argv) {
  SWC::Env::Config::init(argc, argv);
  
  SWC::Env::Clients::init(
    std::make_shared<SWC::client::Clients>(
      nullptr,
      std::make_shared<SWC::client::AppContext>()
    )
  );

  auto settings = SWC::Env::Config::settings();

  SWC::Types::Column col_type = (SWC::Types::Column)settings->get_genum("col-type");
  SWC::Types::KeySeq col_seq = (SWC::Types::KeySeq)settings->get_genum("col-seq");
  size_t ncells = settings->get_i64("ncells");
  uint32_t nfractions = settings->get_i32("nfractions");
  uint32_t cell_versions = settings->get_i32("cell-versions");
  uint32_t value = settings->get_i32("value-size");

  SWC::Test test;

  test.col_type = col_type;
  test.col_seq = col_seq;
  test.col_name = "test-"
                + SWC::Types::to_string(col_type)
                + "-"
                + SWC::Types::to_string(col_seq)
                + "-v" 
                + std::to_string(cell_versions)
                + "-c"
                + std::to_string(ncells)
                + "-f" 
                + std::to_string(nfractions);
  test.cell_versions = cell_versions;
  test.ncells = ncells;
  test.nfractions = nfractions;
  test.run();
  
  SWC::Env::IoCtx::io()->stop();
  std::cout << " ### EXIT ###\n";
  
  exit(0);
}