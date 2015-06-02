// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_TCP_CONNECTION_H_
#define INCLUDE_TCP_CONNECTION_H_

#include <eznetpp.h>
#include <connection.h>

namespace eznetpp {
class tcp_connection : public connection {
 public:
  tcp_connection();
  virtual ~tcp_connection();

  virtual void on_recv(const std::string& msg, int len, int err_no) {}
  virtual void on_send(unsigned int len, int err_no) {}
  virtual void on_close(int err_no) {}

 protected:
  int recv_from_socket(void);
  int send_to_socket(const std::string& data, int len);
  int close_socket(void);

 private:
  DISALLOW_COPY_AND_ASSIGN(tcp_connection);
};
}  // namespace eznetpp

#endif  // INCLUDE_TCP_CONNECTION_H_
