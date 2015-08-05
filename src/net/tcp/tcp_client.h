// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_TCP_CLIENT_H_
#define INCLUDE_TCP_CLIENT_H_

#include "eznetpp.h"
#include "net/if_client.h"

namespace eznetpp {
class connection;
class tcp_client : public if_client {
 public:
  tcp_client() = default;
  virtual ~tcp_client();

  // override
  virtual void on_connect(int client_id) = 0;

  int start_async_io();

  void set_server_info(const std::string ip, int port);
  void set_client_connection(connection* conn);

 protected:
  void* work_thread(void);

 private:
  int setup_client_socket_and_connect();

  int _client_socket = -1;

  std::string _server_ip = "127.0.0.1";
  int _server_port = 6666;

  std::thread _work_th;

  connection* _connection = nullptr;
  
  DISALLOW_COPY_AND_ASSIGN(tcp_client);
};
}  // namespace eznetpp

#endif  // INCLUDE_TCP_CLIENT_H_
