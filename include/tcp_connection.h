// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_TCP_CONNECTION_H_
#define INCLUDE_TCP_CONNECTION_H_

#include <string>

#include "./macros.h"
#include "./common.h"
#include "./connection.h"

namespace eznetpp {
class tcp_connection : public connection {
 public:
  tcp_connection();
  virtual ~tcp_connection();

  virtual void on_read(const std::string& msg, int len, int err_no) {}
  virtual void on_write(unsigned int len, int err_no) {}
  virtual void on_close(int err_no) {}

 protected:
  int read_from_socket(void);
  int write_to_socket(const std::string& data, int len);
  int close_socket(void);

 private:
  DISALLOW_COPY_AND_ASSIGN(tcp_connection);
};
}  // namespace eznetpp

#endif  // INCLUDE_TCP_CONNECTION_H_
