#ifndef TEST_ECHOCLIENT_CONNECTION_H_
#define TEST_ECHOCLIENT_CONNECTION_H_

#include <string>

#include "../include/tcp_connection.h"

class echoclient_connection : public eznetpp::tcp_connection {
 public:
  echoclient_connection();
  virtual ~echoclient_connection();

 public:
  // override
  void on_read(const std::string& msg, int len, int err_no);
  void on_write(unsigned int len, int err_no);
  void on_close(int err_no);
};

#endif  // TEST_ECHOCLIENT_CONNECTION_H_
