// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_EVENT_DISPATCHER_H_
#define INCLUDE_EVENT_DISPATCHER_H_

#include <vector>
#include <string>
#include <mutex>

#include "./macros.h"
//#include "./event.h"

namespace eznetpp {
class if_server;
class event_dispatcher {
 public:
  event_dispatcher(void);
  virtual ~event_dispatcher(void);

 public:
  bool reg_server(if_server* server);
  bool dereg_server(if_server* server);

  bool start_loop();

 private:
  std::vector<if_server*> _server_container;
  //std::vector<event*> _evt_container;

  std::mutex _server_container_mutex;

  DISALLOW_COPY_AND_ASSIGN(event_dispatcher);
};
}  // namespace eznetpp

#endif  // INCLUDE_EVENT_DISPATCHER_H_
