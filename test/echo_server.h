#ifndef TEST_ECHO_SERVER_H_
#define TEST_ECHO_SERVER_H_

#include <string>

#include "../include/tcp_server.h"

class echo_server : public eznetpp::tcp_server {
 public:
  echo_server();
  echo_server(int port, int max_accepts);
  virtual ~echo_server();

 public:
  // override
  void on_accept(int client_id);
};

#endif  // TEST_ECHO_SERVER_H_
