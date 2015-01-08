#ifndef _EZNETPP_SRC_DUMMY_SERVER_SOCKET_H_
#define _EZNETPP_SRC_DUMMY_SERVER_SOCKET_H_

#include <string>

#include "./macros.h"
#include "./if_event_listener.h"

namespace eznetpp {
class dummy_server_socket
  : public if_event_listener {
 public:
  dummy_server_socket();
  virtual ~dummy_server_socket();

 public:
  // override
  virtual void handle_read();
  virtual void handle_write();

 private:
  std::string host_ip;
  int host_port;
  int max_waiting_cnt;
};
}  // namespace eznetpp

#endif  // _EZNETPP_SRC_DUMMY_SERVER_SOCKET_H_
