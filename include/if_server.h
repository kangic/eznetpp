#ifndef _EZNETPP_SRC_IF_SERVER_H_
#define _EZNETPP_SRC_IF_SERVER_H_

#include <string>

#include "macros.h"

namespace eznetpp {
class dummy_connection;
class if_server {
public:
  virtual ~if_server() {}
  
public:
  virtual void on_accept(const dummy_connection& conn) {}
  virtual void on_read(const dummy_connection& conn, const std::string& msg
                     , int len) {}
  virtual void on_write(const dummy_connection& conn, unsigned int len) {}

	virtual int start_async_io() {}
};
  
}  // namespace eznetpp

#endif  // _EZNETPP_SRC_IF_SERVER_H_
