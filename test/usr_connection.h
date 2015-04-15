#ifndef TEST_USR_CONNECTION_H_
#define TEST_USR_CONNECTION_H_

#include <string>

#include "../include/tcp_connection.h"

class usr_connection : public eznetpp::tcp_connection {
 public:
  usr_connection();
  virtual ~usr_connection();

 public:
  // override
  void on_read(const std::string& msg, int len, int err_no);
  void on_write(unsigned int len, int err_no);
  void on_close(int err_no);
};

#endif  // TEST_USR_CONNECTION_H_
