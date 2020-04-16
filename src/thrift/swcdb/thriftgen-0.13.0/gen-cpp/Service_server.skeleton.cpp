// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "Service.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::SWC::Thrift;

class ServiceHandler : virtual public ServiceIf {
 public:
  ServiceHandler() {
    // Your initialization goes here
  }

  void sql_mng_column(const std::string& sql) {
    // Your implementation goes here
    printf("sql_mng_column\n");
  }

  void sql_list_columns(Schemas& _return, const std::string& sql) {
    // Your implementation goes here
    printf("sql_list_columns\n");
  }

  void sql_compact_columns(CompactResults& _return, const std::string& sql) {
    // Your implementation goes here
    printf("sql_compact_columns\n");
  }

  void sql_select(Cells& _return, const std::string& sql) {
    // Your implementation goes here
    printf("sql_select\n");
  }

  void sql_select_rslt_on_column(CCells& _return, const std::string& sql) {
    // Your implementation goes here
    printf("sql_select_rslt_on_column\n");
  }

  void sql_select_rslt_on_key(KCells& _return, const std::string& sql) {
    // Your implementation goes here
    printf("sql_select_rslt_on_key\n");
  }

  void sql_select_rslt_on_fraction(FCells& _return, const std::string& sql) {
    // Your implementation goes here
    printf("sql_select_rslt_on_fraction\n");
  }

  void sql_query(CellsGroup& _return, const std::string& sql, const CellsResult::type rslt) {
    // Your implementation goes here
    printf("sql_query\n");
  }

  void sql_update(const std::string& sql, const int64_t updater_id) {
    // Your implementation goes here
    printf("sql_update\n");
  }

  int64_t updater_create(const int32_t buffer_size) {
    // Your implementation goes here
    printf("updater_create\n");
  }

  void updater_close(const int64_t id) {
    // Your implementation goes here
    printf("updater_close\n");
  }

  void update(const UCCells& cells, const int64_t updater_id) {
    // Your implementation goes here
    printf("update\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  ::std::shared_ptr<ServiceHandler> handler(new ServiceHandler());
  ::std::shared_ptr<TProcessor> processor(new ServiceProcessor(handler));
  ::std::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  ::std::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  ::std::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}
