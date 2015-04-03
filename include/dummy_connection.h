// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_DUMMY_CONNECTION_H_
#define INCLUDE_DUMMY_CONNECTION_H_

#include <string>

#include "./macros.h"
#include "./connection.h"

namespace eznetpp {
class dummy_connection : public connection {
 public:
  dummy_connection();
  virtual ~dummy_connection();

 public:
  int read(std::string data, int* received_len);
  int write(const std::string& data, int len);
};
}  // namespace eznetpp

#endif  // INCLUDE_DUMMY_CONNECTION_H_
