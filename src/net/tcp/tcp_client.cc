//  Copyright [2015] <kangic21@gmail.com>

#include "tcp_client.h"

#include "tcp_connection.h"

namespace eznetpp {

tcp_client::~tcp_client(void) {
  _connection = nullptr;
}

int tcp_client::start_async_io() {
  logger::instance().log(logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "S ->");

  // initialization
  int ret = setup_client_socket_and_connect();
  if (ret != 0) {
    return ret;
  }

  on_connect(_client_socket);

  _work_th = std::thread(&tcp_client::work_thread, this);

  if (!_work_th.joinable()) {
    logger::instance().log(logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "failed to create work thread(%d)\n"
                           , errno);
    
    return -1;
  }

  logger::instance().log(logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "E <-");

  return 0;
}

int tcp_client::setup_client_socket_and_connect() {
  _client_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (_client_socket == -1) {
    return _client_socket;
  }

  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(_server_ip.c_str());
  server_addr.sin_port = htons(_server_port);

  // connect to server(on_connect event)
  int ret = connect(_client_socket, (struct sockaddr *)&server_addr
                    , sizeof(server_addr));
  if (ret == -1) {
    logger::instance().log(logger::log_level::error
                           , __FILE__, __FUNCTION__, __LINE__
                           , "connect error : %d", errno);

    close(_client_socket);
    _client_socket = -1;
    return errno;
  }

  return 0;
}

void tcp_client::set_server_info(const std::string ip, int port) {
  _server_ip = ip;
  _server_port = port;
}

void tcp_client::set_client_connection(connection* conn) {
  _connection = conn;
}

// work thread
void* tcp_client::work_thread(void) {
  logger::instance().log(logger::log_level::debug
                         , __FILE__, __FUNCTION__, __LINE__
                         , "start work_thread");

  while (1) {
    int ret = _connection->recv_from_socket();

    if (ret <= 0) {
      _connection->close_socket();
      logger::instance().log(logger::log_level::debug
                             , __FILE__, __FUNCTION__, __LINE__
                             , "Close fd %d"
                             , _connection->socket_id());

      if (_connection != nullptr) {
        delete _connection;
        _connection = nullptr;
      }

      break;
    } else {
      logger::instance().log(logger::log_level::debug
                             , __FILE__, __FUNCTION__, __LINE__
                             , "read %d bytes"
                             , ret);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  return nullptr;
}

}  // namespace eznetpp
