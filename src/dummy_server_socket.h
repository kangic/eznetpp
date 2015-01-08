#ifndef _EZNETPP_SRC_DUMMY_SERVER_SOCKET_H_
#define _EZNETPP_SRC_DUMMY_SERVER_SOCKET_H_

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
  DISALLOW_COPY_AND_ASSIGN(dummy_server_socket);
};
}  // namespace eznetpp

#endif  // _EZNETPP_SRC_DUMMY_SERVER_SOCKET_H_
