/*
 * SWC-DB© Copyright since 2019 Alex Kashirin <kashirin.alex@gmail.com>
 * License details at <https://github.com/kashirin-alex/swc-db/#license>
 */


#include "swcdb/core/config/Settings.h"
#include "swcdb/core/comm/SerializedClient.h"
#include <queue>
#include <unordered_map>


namespace SWC { namespace Comm { namespace client {


SWC_SHOULD_INLINE
ServerConnections::ServerConnections(const std::string& srv_name,
                                     const EndPoint& endpoint,
                                     const IoContextPtr& ioctx,
                                     const AppContext::Ptr& ctx,
                                     ConfigSSL* ssl_cfg)
                                    : m_srv_name(srv_name),
                                      m_endpoint(endpoint),
                                      m_ioctx(ioctx), m_ctx(ctx),
                                      m_ssl_cfg(ssl_cfg) {
}

void ServerConnections::reusable(ConnHandlerPtr& conn, bool preserve) {
  while(pop(&(conn = nullptr)) && !conn->is_open());
  if(preserve && conn)
    push(conn);
  /*
  if(conn)
    SWC_LOGF(LOG_DEBUG, "Reusing connection: %s, %s",
             m_srv_name.c_str(), to_string(conn).c_str());
  */
}

void ServerConnections::connection(ConnHandlerPtr& conn,
                                   const std::chrono::milliseconds&,
                                   bool preserve) {

  SWC_LOG_OUT(LOG_DEBUG,
    SWC_LOG_OSTREAM << "Connecting Sync: " << m_srv_name << ' '
      << m_endpoint << ' ' << (m_ssl_cfg ? "SECURE" : "PLAIN"); );

  asio::ip::tcp::socket sock(m_ioctx->executor());
  asio::error_code ec;
  sock.open(m_endpoint.protocol(), ec);
  if(ec || !sock.is_open())
    return;

  sock.connect(m_endpoint, ec);
  if(ec || !sock.is_open())
    return;

  try {
    conn = m_ssl_cfg && sock.remote_endpoint().address()
                         != sock.local_endpoint().address()
      ? m_ssl_cfg->make_client(m_ctx, sock, ec)
      : std::make_shared<ConnHandlerPlain>(m_ctx, sock);
  } catch(...) {
    SWC_LOG_CURRENT_EXCEPTION("");
    conn = nullptr;
    sock.close(ec);
    return;
  }
  if(!ec && conn->is_open()) {
    conn->new_connection();
    if(preserve)
      push(conn);
    // SWC_LOGF(LOG_DEBUG, "New connection: %s, %s",
    //          m_srv_name.c_str(), to_string(conn).c_str());
  } else {
    conn = nullptr;
  }
}

void ServerConnections::connection(const std::chrono::milliseconds&,
                                   NewCb_t&& cb, bool preserve) {

  SWC_LOG_OUT(LOG_DEBUG,
    SWC_LOG_OSTREAM << "Connecting Async: " << m_srv_name << ' '
      << m_endpoint << ' ' << (m_ssl_cfg ? "SECURE" : "PLAIN"); );

  auto sock = std::make_shared<asio::ip::tcp::socket>(m_ioctx->executor());
  sock->async_connect(
    m_endpoint,
    [sock, preserve, cb=std::move(cb), ptr=shared_from_this()]
    (const std::error_code& ec) {
      if(ec || !sock->is_open()) {
        cb(nullptr);
        return;
      }
      try {
        if(ptr->m_ssl_cfg && sock->remote_endpoint().address()
                              != sock->local_endpoint().address()) {
          ptr->m_ssl_cfg->make_client(
            ptr->m_ctx, *sock.get(),
            [preserve, ptr, cb=std::move(cb)]
            (const ConnHandlerPtr& conn, const std::error_code& ec) {
              if(ec || !conn->is_open()) {
                cb(nullptr);
              } else {
                conn->new_connection();
                if(preserve)
                  ptr->push(conn);
                cb(conn);
              }
            }
          );

        } else {
          auto conn =
            std::make_shared<ConnHandlerPlain>(ptr->m_ctx, *sock.get());
          conn->new_connection();
          if(preserve)
            ptr->push(conn);
          cb(conn);
        }
      } catch(...) {
        SWC_LOG_CURRENT_EXCEPTION("");
        std::error_code ec;
        sock->close(ec);
        cb(nullptr);
      }
    }
  );
}

void ServerConnections::close_all() {
  ConnHandlerPtr conn;
  while(pop(&conn)) conn->do_close();
}


Serialized::Serialized(std::string&& srv_name, const IoContextPtr& ioctx,
                       const AppContext::Ptr& ctx)
            : m_srv_name(std::move(srv_name)), m_ioctx(ioctx), m_ctx(ctx),
              m_use_ssl(Env::Config::settings()->get_bool("swc.comm.ssl")),
              m_ssl_cfg(m_use_ssl ? new ConfigSSL() : nullptr),
              m_run(true) {
  SWC_LOGF(LOG_DEBUG, "Init: %s", m_srv_name.c_str());
}

ServerConnections::Ptr Serialized::get_srv(const EndPoint& endpoint) {
  size_t hash = endpoint_hash(endpoint);
  iterator it;
  Core::MutexSptd::scope lock(m_mutex);
  if((it = find(hash)) == end())
    it = emplace(
      hash,
      std::make_shared<ServerConnections>(
        m_srv_name, endpoint, m_ioctx, m_ctx,
        m_use_ssl && m_ssl_cfg->need_ssl(endpoint) ? m_ssl_cfg : nullptr
      )
    ).first;

  return it->second;
}

ConnHandlerPtr Serialized::get_connection(
      const EndPoints& endpoints,
      const std::chrono::milliseconds& timeout,
      uint32_t probes, bool preserve) noexcept {
  try {
    return _get_connection(endpoints, timeout, probes, preserve);
  } catch (...) {
    SWC_LOG_CURRENT_EXCEPTION("");
    return nullptr;
  }
}

ConnHandlerPtr Serialized::_get_connection(
      const EndPoints& endpoints,
      const std::chrono::milliseconds& timeout,
      uint32_t probes, bool preserve) {

  ConnHandlerPtr conn = nullptr;
  if(endpoints.empty()) {
    SWC_LOGF(LOG_WARN, "get_connection: %s, Empty-Endpoints",
                        m_srv_name.c_str());
    return conn;
  }

  ServerConnections::Ptr srv;
  uint32_t tries = probes;
  if(m_run && m_ioctx->running) do {

    for(auto& endpoint : endpoints){
      srv = get_srv(endpoint);
      srv->reusable(conn, preserve);
      if(!conn)
        srv->connection(conn, timeout, preserve);
      if(conn)
        return conn;
    }
    SWC_LOGF(LOG_DEBUG, "get_connection: %s, tries=%u",
                         m_srv_name.c_str(), tries);

    std::this_thread::sleep_for(
      std::chrono::milliseconds(3000)); // ? cfg-setting

  } while (m_run && m_ioctx->running && (!probes || --tries));

  return conn;
}

void Serialized::get_connection(
      const EndPoints& endpoints,
      ServerConnections::NewCb_t&& cb,
      const std::chrono::milliseconds& timeout,
      uint32_t probes, bool preserve) noexcept {
  try {
    if(endpoints.empty()) {
      SWC_LOGF(LOG_WARN, "get_connection: %s, Empty-Endpoints",
                          m_srv_name.c_str());
    } else {
      _get_connection(
        endpoints, std::move(cb), timeout, probes, probes, 0, preserve);
      return;
    }
  } catch (...) {
    SWC_LOG_CURRENT_EXCEPTION("");
  }
  cb(nullptr);
}

void Serialized::_get_connection(
      const EndPoints& endpoints,
      ServerConnections::NewCb_t&& cb,
      const std::chrono::milliseconds& timeout,
      uint32_t probes, uint32_t tries,
      size_t next, bool preserve) {

  if(!m_run || !m_ioctx->running) {
    cb(nullptr);
    return;
  }

  if(next == endpoints.size())
    next = 0;

  auto srv = get_srv(endpoints.at(next));
  ConnHandlerPtr conn = nullptr;
  srv->reusable(conn, preserve);
  if(conn || (probes && !tries)) {
    cb(conn);
    return;
  }

  ++next;
  SWC_LOGF(LOG_DEBUG, "get_connection: %s, tries=%u",
           m_srv_name.c_str(), tries);
  srv->connection(timeout,
    [endpoints, timeout, probes, tries, next, preserve,
     cb=std::move(cb), ptr=shared_from_this()]
    (const ConnHandlerPtr& conn) mutable {
      if(!ptr->m_run.load() || (conn && conn->is_open())) {
        cb(conn);
        return;
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // ? cfg-setting
      try {
        ptr->_get_connection(
          endpoints, std::move(cb), timeout,
          probes, next == endpoints.size() ? tries-1 : tries,
          next,
          preserve
        );
      } catch (...) {
        SWC_LOG_CURRENT_EXCEPTION("");
        cb(nullptr);
      }
    },
    preserve
  );
}

void Serialized::preserve(ConnHandlerPtr& conn) {
  size_t hash = conn->endpoint_remote_hash();
  iterator it;
  Core::MutexSptd::scope lock(m_mutex);
  if((it = find(hash)) != end())
    it->second->push(conn);
}

void Serialized::close(ConnHandlerPtr& conn) {
  size_t hash = conn->endpoint_remote_hash();
  conn->do_close();
  iterator it;
  Core::MutexSptd::scope lock(m_mutex);
  if((it = find(hash)) != end() && it->second->empty())
    erase(it);
}

IoContextPtr Serialized::io() noexcept {
  return m_ioctx;
}

void Serialized::print(std::ostream& out, ConnHandlerPtr& conn) {
  conn->print(out << m_srv_name << ' ');
}

void Serialized::stop() {
  m_run.store(false);

  iterator it;
  for(ServerConnections::Ptr srv;;) {
    {
      Core::MutexSptd::scope lock(m_mutex);
      if((it = begin()) == end())
        break;
      srv = it->second;
      erase(it);
    }
    srv->close_all();
  }
  SWC_LOGF(LOG_INFO, "Stop: %s", m_srv_name.c_str());
}

Serialized::~Serialized() {
  if(m_ssl_cfg)
    delete m_ssl_cfg;
}



}}}
