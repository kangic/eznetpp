#ifndef TEST_USR_SERVER_H_
#define TEST_USR_SERVER_H_

#include <string>

#include "../include/tcp_server.h"

class usr_server : public eznetpp::tcp_server {
 public:
  usr_server();
  usr_server(int port, int max_accepts);
  virtual ~usr_server();

 public:
  // override
  void on_accept(int client_id);
};

#endif  // TEST_USR_SERVER_H_
