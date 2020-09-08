/*
 * Copyright Since 2019 SWC-DB© [author: Kashirin Alex kashirin.alex@gmail.com]
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#include "swcdb/utils/cli/Shell_Ranger.h"
#include "swcdb/db/client/Clients.h"
#include "swcdb/db/client/AppContext.h"
#include "swcdb/db/Protocol/Rgr/req/Report.h"
#include "swcdb/db/client/sql/Reader.h"


namespace SWC { namespace Utils { namespace shell {


Rgr::Rgr() 
  : Interface("\033[32mSWC-DB(\033[36mrgr\033[32m)\033[33m> \033[00m",
              "/tmp/.swc-cli-ranger-history") {

  options.push_back(
    new Option(
      "report-resources", 
      {"report Ranger resources",
      "report-resources endpoint/hostname[|port];"},
      [ptr=this](std::string& cmd){return ptr->report_resources(cmd);}, 
      new re2::RE2("(?i)^(report-resources)")
    )
  );

  options.push_back(
    new Option(
      "report", 
      {"report loaded column or all and opt. ranges on a Ranger",
      "report [cid=NUM/column='name'] [ranges] endpoint/hostname[|port];"},
      [ptr=this](std::string& cmd){return ptr->report(cmd);}, 
      new re2::RE2("(?i)^(report)")
    )
  );

  Env::Clients::init(
    std::make_shared<client::Clients>(
      nullptr,
      std::make_shared<client::AppContext>()
    )
  );
}

bool Rgr::read_endpoint(std::string& host_or_ips, EndPoints& endpoints) {
  host_or_ips.erase(
    std::remove_if(host_or_ips.begin(), host_or_ips.end(), 
                  [](unsigned char x){return std::isspace(x);} ), 
    host_or_ips.end()
  );
  std::string message;

  uint32_t port = 0;
  size_t at = host_or_ips.find_first_of("|");
  if(at != std::string::npos) {
    std::string port_str = host_or_ips.substr(at+1);
    host_or_ips = host_or_ips.substr(0, at);
    try {
      if((port = std::stol(port_str)) > UINT16_MAX )
        err = ERANGE;
    } catch(...) {
      const Exception& e = SWC_CURRENT_EXCEPTION("");
      err = e.code();
    }
    if(err) {
      message.append("Bad value='"+port_str+ "' for port\n");
      return error(message);
    }
  } else {
    port = Env::Config::settings()->get_i16("swc.rgr.port");
  }
  if(!port) {
    message.append("Bad value='"+std::to_string(port)+ "' for port\n");
    return error(message);
  }
  
  std::vector<std::string> ips;
  std::string host;
  if(Resolver::is_ipv4_address(host_or_ips) || Resolver::is_ipv6_address(host_or_ips))
    ips.push_back(host_or_ips);
  else
    host = host_or_ips;

  try {
    endpoints = Resolver::get_endpoints(port, ips, host, {}, false);
    if(endpoints.empty()) {
      message.append("Empty endpoints\n");
      err = EINVAL;
    }
  } catch(...) {
    const Exception& e = SWC_CURRENT_EXCEPTION("");
    err = e.code();
    message.append(e.what());
    message.append("\n");
  }
  return err ? error(message) : true;
}

bool Rgr::report_resources(std::string& cmd) {
  
  size_t at = cmd.find_first_of(" ");
  std::string host_or_ips = cmd.substr(at+1);

  EndPoints endpoints;
  bool r = read_endpoint(host_or_ips, endpoints);
  if(err)
    return r;

  std::promise<void>  r_promise;
  Protocol::Rgr::Req::ReportRes::request(
    endpoints, 
    [this, await=&r_promise] 
    (const client::ConnQueue::ReqBase::Ptr&, const int& error,
     const Protocol::Rgr::Params::Report::RspRes& rsp) {
      if(!(err = error)) {
        SWC_PRINT << "";
        rsp.display(SWC_LOG_OSTREAM);
        SWC_LOG_OSTREAM << SWC_PRINT_CLOSE;
      }
      await->set_value();
    }
  );
  r_promise.get_future().wait();

  if(err) {
    std::string message(Error::get_text(err));
    message.append("\n");
    return error(message);
  }
  return true;
}

bool Rgr::report(std::string& cmd) {
  std::string message;

  size_t at = cmd.find_first_of(" ");
  cmd = cmd.substr(at+1);
  cid_t cid = DB::Schema::NO_CID;

  client::SQL::Reader reader(cmd, message);
  while(reader.found_space());
  if(reader.found_token("cid", 3)) {
    reader.expect_eq();
    if(!reader.err) {
      int64_t _cid;
      bool was_set = false;
      reader.read_int64_t(_cid, was_set);
      if(was_set)
        cid = _cid;
    }
    if(reader.err)
      return error(message);

  } else if(reader.found_token("column", 6)) {
    reader.expect_eq();
    if(reader.err)
      return error(message);
    
    std::string col_name;
    reader.read(col_name, " ");
    if(col_name.empty())
      return error("Missing Column Name");
    
    auto schema = reader.get_schema(col_name);
    if(reader.err)
      return error(message);
    cid = schema->cid;
  }

  while(reader.found_space());
  bool ranges = reader.found_token("ranges", 6);

  while(reader.found_space());
  std::string host_or_ips(reader.ptr, reader.remain);
  EndPoints endpoints;
  bool r = read_endpoint(host_or_ips, endpoints);
  if(err)
    return r;

  auto func = cid == DB::Schema::NO_CID 
    ? (ranges ? Protocol::Rgr::Params::Report::Function::COLUMNS_RANGES
              : Protocol::Rgr::Params::Report::Function::CIDS)
    : (ranges ? Protocol::Rgr::Params::Report::Function::COLUMN_RANGES
              : Protocol::Rgr::Params::Report::Function::COLUMN_RIDS);

  std::promise<void>  r_promise;

  switch(func) {
    case Protocol::Rgr::Params::Report::Function::COLUMNS_RANGES: {
      Protocol::Rgr::Req::ReportColumnsRanges::request(
        endpoints, 
        [this, await=&r_promise] 
        (const client::ConnQueue::ReqBase::Ptr&, const int& error,
         const Protocol::Rgr::Params::Report::RspColumnsRanges& rsp) {
          if(!(err = error)) {
            SWC_PRINT << "";
            rsp.display(SWC_LOG_OSTREAM);
            SWC_LOG_OSTREAM << SWC_PRINT_CLOSE;
          }
          await->set_value();
        }
      );
      break;
    }
    case Protocol::Rgr::Params::Report::Function::CIDS: {
      Protocol::Rgr::Req::ReportCids::request(
        endpoints, 
        [this, await=&r_promise] 
        (const client::ConnQueue::ReqBase::Ptr&, const int& error,
         const Protocol::Rgr::Params::Report::RspCids& rsp) {
          if(!(err = error)) {
            SWC_PRINT << "";
            rsp.display(SWC_LOG_OSTREAM);
            SWC_LOG_OSTREAM << SWC_PRINT_CLOSE;
          }
          await->set_value();
        }
      );
      break;
    }
    case Protocol::Rgr::Params::Report::Function::COLUMN_RANGES: {
      Protocol::Rgr::Req::ReportColumnsRanges::request(
        endpoints, 
        cid,
        [this, await=&r_promise]
        (const client::ConnQueue::ReqBase::Ptr&, const int& error,
         const Protocol::Rgr::Params::Report::RspColumnsRanges& rsp) {
          if(!(err = error)) {
            SWC_PRINT << "";
            rsp.display(SWC_LOG_OSTREAM);
            SWC_LOG_OSTREAM << SWC_PRINT_CLOSE;
          }
          await->set_value();
        }
      );
      break;
    }
    case Protocol::Rgr::Params::Report::Function::COLUMN_RIDS: {
      Protocol::Rgr::Req::ReportColumnRids::request(
        endpoints, 
        cid,
        [this, await=&r_promise]
        (const client::ConnQueue::ReqBase::Ptr&, const int& error,
         const Protocol::Rgr::Params::Report::RspColumnRids& rsp) {
          if(!(err = error)) {
            SWC_PRINT << "";
            rsp.display(SWC_LOG_OSTREAM);
            SWC_LOG_OSTREAM << SWC_PRINT_CLOSE;
          }
          await->set_value();
        }
      );
      break;
    }
    default: {
      err = Error::NOT_IMPLEMENTED;
      r_promise.set_value();
      break;
    }
  }

  r_promise.get_future().wait();

  if(err) {
    message.append(Error::get_text(err));
    message.append("\n");
    return error(message);
  }
  return true;
}



}}} // namespace Utils::shell