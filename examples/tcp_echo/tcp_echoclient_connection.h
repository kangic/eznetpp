#ifndef TEST_TCP_ECHOCLIENT_CONNECTION_H_
#define TEST_TCP_ECHOCLIENT_CONNECTION_H_

#include <string>

#include "../include/tcp_connection.h"

class tcp_echoclient_connection : public eznetpp::tcp_connection {
 public:
  tcp_echoclient_connection();
  virtual ~tcp_echoclient_connection();

 public:
  // override
  void on_recv(const std::string& msg, int len, int err_no);
  void on_send(unsigned int len, int err_no);
  void on_close(int err_no);
};

#endif  // TEST_TCP_ECHOCLIENT_CONNECTION_H_
