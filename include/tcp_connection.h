// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_TCP_CONNECTION_H_
#define INCLUDE_TCP_CONNECTION_H_

#include <string>

#include "./macros.h"
#include "./connection.h"

namespace eznetpp {
class tcp_connection : public connection {
 public:
  tcp_connection();
  virtual ~tcp_connection();

 public:
  // override
  int read(std::string data, int* received_len);
  int write(const std::string& data, int len);
  int close();

 private:
  DISALLOW_COPY_AND_ASSIGN(tcp_connection);
};
}  // namespace eznetpp

#endif  // INCLUDE_TCP_CONNECTION_H_
