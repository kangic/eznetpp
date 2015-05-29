#ifndef TEST_ECHO_CLIENT_H_
#define TEST_ECHO_CLIENT_H_

#include <string>

#include "../include/tcp_client.h"

class echo_client : public eznetpp::tcp_client {
 public:
  echo_client();
  virtual ~echo_client();

 public:
  // override
  void on_connect(int client_id);
};

#endif  // TEST_ECHO_CLIENT_H_
