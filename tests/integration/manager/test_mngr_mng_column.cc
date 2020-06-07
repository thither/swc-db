/*
 * Copyright Since 2019 SWC-DB© [author: Kashirin Alex kashirin.alex@gmail.com]
 */
 
#include "swcdb/core/config/Settings.h"
#include "swcdb/core/comm/Settings.h"

#include "swcdb/db/client/Clients.h"
#include "swcdb/db/client/AppContext.h"

#include "swcdb/db/Cells/SpecsScan.h"

#include "swcdb/db/Protocol/Mngr/req/ColumnMng.h"
#include "swcdb/db/Protocol/Mngr/req/ColumnGet.h"
#include "swcdb/db/Protocol/Mngr/req/RgrGet.h"

#include "swcdb/db/client/Stats/Stat.h"


namespace SWC{ namespace Config {

void Settings::init_app_options(){
  init_comm_options();
  init_client_options();
  // file_desc.add_options();
}
void Settings::init_post_cmd_args(){ }
}}

using namespace SWC;

struct ExpctedRsp final {
  public:
  ExpctedRsp(std::string name, Types::Encoding blk_encoding, bool exists)
            : name(name), blk_encoding(blk_encoding),
              exists(exists), chks(0) { }

  std::string name;
  Types::Encoding blk_encoding;
  bool exists;
  std::atomic<int> chks;
};

std::string get_name(int n, bool modified=false){
  std::string name("column-");
  name.append(std::to_string(n));
  if(modified)
    name.append("-modified");
  return name;
}


void check_delete(int num_of_cols, bool modified) {
  std::atomic<int> count = num_of_cols;
  std::condition_variable cv;
  std::mutex mutex;

  for(int n=1; n<=num_of_cols; ++n) {
    Protocol::Mngr::Req::ColumnGet::schema(
      get_name(n, modified), 
      [&cv, &count] 
      (client::ConnQueue::ReqBase::Ptr req_ptr, 
       int err, const Protocol::Mngr::Params::ColumnGetRsp& rsp) {
        if(err)
          SWC_PRINT << "ColumnGet err=" << err 
                    << "(" << Error::get_text(err) << ")" 
                    << SWC_PRINT_CLOSE;
        if(err == Error::REQUEST_TIMEOUT)
          return req_ptr->request_again();
        if(err) {
          --count;
          cv.notify_all();
          return;
        }
        
        Protocol::Mngr::Req::ColumnMng::request(
          Protocol::Mngr::Req::ColumnMng::Func::DELETE,
          rsp.schema,
          [&cv, &count](client::ConnQueue::ReqBase::Ptr req_ptr, int err){
            if(err)
              SWC_PRINT << "ColumnMng DELETE err=" << err 
                        << "(" << Error::get_text(err) << ")" 
                        << SWC_PRINT_CLOSE;
            if(err == Error::REQUEST_TIMEOUT)
              return req_ptr->request_again();
            --count;
            cv.notify_one();
          },
          300000
        );
        
      },
      300000
    );
    /*
    if(n % 100) {
      std::unique_lock lock_wait(mutex);
      cv.wait(lock_wait, [&count]() { return count.load() % 100 == 0; });
    }
    */
  }
  
  std::unique_lock lock_wait(mutex);
  cv.wait(lock_wait, [&count]() {
      SWC_PRINT << "check_delete waiting=" << count << SWC_PRINT_CLOSE; 
      return !count; 
    }
  );
}

void check_get(int num_of_cols, bool modified, Types::Encoding blk_encoding, bool exist = true, bool verbose=false){
  std::cout << "########### get_schema_by_name ###########\n";
  std::shared_ptr<Stats::Stat> latency = std::make_shared<Stats::Stat>();
  
  std::vector<std::shared_ptr<ExpctedRsp>> expected;

  for(int n=1; n<=num_of_cols;++n){
    expected.push_back(std::make_shared<ExpctedRsp>(
      get_name(n, modified), 
      blk_encoding,
      exist
    ));
  }

  for(auto& req : expected){
    Protocol::Mngr::Req::ColumnGet::schema(
      req->name, 
      [req, latency, verbose, start_ts=std::chrono::system_clock::now()]
      (client::ConnQueue::ReqBase::Ptr req_ptr, 
        int err, const Protocol::Mngr::Params::ColumnGetRsp& rsp) {

        if(err == Error::REQUEST_TIMEOUT) {
          std::cout << " err=" << err << "(" << Error::get_text(err) << ") \n";
          req_ptr->request_again();
          return;
        }
        
        uint64_t took  = std::chrono::duration_cast<std::chrono::nanoseconds>(
                          std::chrono::system_clock::now() - start_ts).count();
        latency->add(took);
        if(verbose)
          std::cout << "ColumnGetRsp: exists="<< req->exists << " took=" << took  
                    << " count=" << latency->count()
                    << " err=" << err << "(" << Error::get_text(err) << ") " 
                    << " " << (err==Error::OK?rsp.schema->to_string().c_str():"NULL") << "\n";

        if(err==Error::OK){
          if(!req->exists) {
            std::cerr << " SHOULDN'T exist name=" << req->name << "\n";
            exit(1); 
          }
          if(req->blk_encoding != rsp.schema->blk_encoding) {
            std::cerr << " blk_encoding don't match \n";
            exit(1); 
          }
          if(req->name.compare(rsp.schema->col_name) != 0) {
            std::cerr << " name don't match \n";
            exit(1); 
          }
        } else if(req->exists){
          std::cerr << " SHOULD exist name=" << req->name << "\n";
          exit(1);  
        }
        ++req->chks;
      },
      300000
    );
  }
  
  while(latency->count() < num_of_cols) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  std::cout << "get_schema_by_name"
            << " avg=" << latency->avg()
            << " min=" << latency->min()
            << " max=" << latency->max()
            << " count=" << latency->count() 
            << "\n";


  std::cout << "########### get_id_by_name ###########\n";
  latency = std::make_shared<Stats::Stat>();

  for(auto& req : expected){
    Protocol::Mngr::Req::ColumnGet::cid(
      req->name, 
      [req, latency, verbose, start_ts=std::chrono::system_clock::now()]
      (client::ConnQueue::ReqBase::Ptr req_ptr,
       int err, const Protocol::Mngr::Params::ColumnGetRsp& rsp) {
        
        if(err == Error::REQUEST_TIMEOUT) {
          std::cout << " err=" << err << "(" << Error::get_text(err) << ") \n";
          req_ptr->request_again();
          return;
        }

        uint64_t took  = std::chrono::duration_cast<std::chrono::nanoseconds>(
                          std::chrono::system_clock::now() - start_ts).count();
        latency->add(took);

        if(verbose)
          std::cout << "ColumnGetRsp: exists="<< req->exists << " took=" << took  
                    << " count=" << latency->count()
                    << " err=" << err << "(" << Error::get_text(err) << ") " 
                    << " " << (err==Error::OK?rsp.cid:-1) << "\n";

        if(err==Error::OK){
          if(!req->exists) {
            std::cerr << " SHOULDN'T exist name=" << req->name << "\n";
            exit(1); 
          }
        } else if(req->exists){
          std::cerr << " SHOULD exist name=" << req->name << "\n";
          exit(1);  
        }
        ++req->chks;

      },
      300000
    );
  }

  while(latency->count() < num_of_cols) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  std::cout << "get get_id_by_name"
            << " avg=" << latency->avg()
            << " min=" << latency->min()
            << " max=" << latency->max()
            << " count=" << latency->count() 
            << "\n";

  for(auto& req : expected) {
    if(req->chks != 2){
      
      std::cerr << " chks="<< req->chks.load()
                << " name=" << req->name 
                << " exists=" << req->exists 
                << " \n";
      exit(1);
    }
  }
}


void chk(Protocol::Mngr::Req::ColumnMng::Func func, int num_of_cols, 
         Types::Encoding blk_encoding, bool modified, bool verbose=false) {

  std::cout << "########### chk func=" << func << " ###########\n";
  std::shared_ptr<Stats::Stat> latency = std::make_shared<Stats::Stat>();

  for(int n=1;n<=num_of_cols;++n) {
    
    auto schema = DB::Schema::make();
    schema->col_name = get_name(n, modified);
    schema->col_type = Types::Column::COUNTER_I64;
    schema->cell_versions = 10; 
    schema->cell_ttl = 1234; 
    schema->blk_encoding = blk_encoding; 
    schema->blk_size = 3; 
    schema->blk_cells = 9876543;

    Protocol::Mngr::Req::ColumnMng::request(
      func, schema,
      [func, latency, verbose, start_ts=std::chrono::system_clock::now()]
      (client::ConnQueue::ReqBase::Ptr req_ptr, int err){

        uint64_t took 
          = std::chrono::duration_cast<std::chrono::nanoseconds>(
              std::chrono::system_clock::now() - start_ts).count();

        if(err != Error::OK && (
            (func == Protocol::Mngr::Req::ColumnMng::Func::CREATE 
              && err != Error::COLUMN_SCHEMA_NAME_EXISTS)
            || 
            (func == Protocol::Mngr::Req::ColumnMng::Func::DELETE 
              && err != Error::COLUMN_SCHEMA_NAME_NOT_EXISTS 
              && err != Error::COLUMN_SCHEMA_NAME_NOT_CORRES)
            || 
            (func == Protocol::Mngr::Req::ColumnMng::Func::MODIFY 
              && err != Error::COLUMN_SCHEMA_NAME_NOT_EXISTS
              && err != Error::COLUMN_SCHEMA_NOT_DIFFERENT )
          )) {
          SWC_PRINT << " func = " << func 
                    << " err="<<err<< "(" << Error::get_text(err) << ")" << SWC_PRINT_CLOSE;
          req_ptr->request_again();
        } else {
          latency->add(took);
        }
            
        if(verbose)
          SWC_PRINT << " func = " << func 
            << " err="<<err<< "(" << Error::get_text(err) << ")"
            << " took=" << took
            << " avg=" << latency->avg()
            << " min=" << latency->min()
            << " max=" << latency->max()
            << " count=" << latency->count()
            << SWC_PRINT_CLOSE;
      },
      300000
      );
  
  }
  
  while(latency->count() < num_of_cols) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  std::cout << " func="<< func 
            << " num_of_cols="<< num_of_cols
            << " avg=" << latency->avg()
            << " min=" << latency->min()
            << " max=" << latency->max()
            << " count=" << latency->count()
            << "\n";
}

void chk_rename(int num_of_cols, bool verbose=false){
  std::cout << "########### chk_rename ###########\n";
  std::shared_ptr<Stats::Stat> latency = std::make_shared<Stats::Stat>();
  
  std::vector<std::shared_ptr<ExpctedRsp>> expected;

  for(int n=1; n<=num_of_cols;++n){
    std::string name = get_name(n, false);
    Protocol::Mngr::Req::ColumnGet::schema(
      name, 
      [n, latency, verbose, start_ts=std::chrono::system_clock::now()]
      (client::ConnQueue::ReqBase::Ptr req_ptr, 
       int err, const Protocol::Mngr::Params::ColumnGetRsp& rsp) {

        if(err == Error::REQUEST_TIMEOUT) {
          SWC_PRINT << " err=" << err << "(" << Error::get_text(err) << ")" << SWC_PRINT_CLOSE;
          req_ptr->request_again();
          return;
        }
        
        auto new_schema = DB::Schema::make(rsp.schema);
        new_schema->col_name = get_name(n, true);
        new_schema->revision = 2;

        if(verbose)
          SWC_PRINT << "modify name: \n" 
            << "from " << rsp.schema->to_string() << "\n"
            << "to   " << new_schema->to_string() << SWC_PRINT_CLOSE;

        Protocol::Mngr::Req::ColumnMng::request(
          Protocol::Mngr::Req::ColumnMng::Func::MODIFY,
          new_schema,
          [latency, start_ts](client::ConnQueue::ReqBase::Ptr req_ptr, int err){
            if(err != Error::OK 
              && err != Error::COLUMN_SCHEMA_NAME_NOT_EXISTS
              && err != Error::COLUMN_SCHEMA_NOT_DIFFERENT){
              
              req_ptr->request_again();
              return;
            }
            uint64_t took  = std::chrono::duration_cast<std::chrono::nanoseconds>(
                              std::chrono::system_clock::now() - start_ts).count();
            latency->add(took);
          },
          300000
        );
      }
    ); 
  }

  while(latency->count() < num_of_cols) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  
  SWC_PRINT << " modify name "
            << " num_of_cols="<< num_of_cols
            << " avg=" << latency->avg()
            << " min=" << latency->min()
            << " max=" << latency->max()
            << " count=" << latency->count()
            << SWC_PRINT_CLOSE;
}


int main(int argc, char** argv) {
  Env::Config::init(argc, argv);
  
  Env::Clients::init(
    std::make_shared<client::Clients>(
      nullptr,
      std::make_shared<client::AppContext>()
    )
  );
  

  int num_of_cols = 1000;

  check_delete(num_of_cols, false);
  check_delete(num_of_cols, true);

  chk(Protocol::Mngr::Req::ColumnMng::Func::CREATE, num_of_cols, Types::Encoding::PLAIN, false);
  check_get(num_of_cols, false, Types::Encoding::PLAIN);
  std::cout << "\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));


  chk_rename(num_of_cols, true);

  check_get(num_of_cols, true, Types::Encoding::PLAIN, true);
  std::cout << "\n";

  chk(Protocol::Mngr::Req::ColumnMng::Func::MODIFY, num_of_cols, Types::Encoding::SNAPPY, true);
  check_get(num_of_cols, true, Types::Encoding::SNAPPY);
  std::cout << "\n";



  check_delete(num_of_cols, true);
  check_get(num_of_cols, true, Types::Encoding::SNAPPY, false);
  std::cout << "\n";

  std::cout << " OK! \n \n";


  exit(0);


  Env::IoCtx::io()->stop();
  std::cout << " ### EXIT ###\n";
  return 0;
}