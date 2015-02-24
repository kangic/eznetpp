#ifndef _EZNETPP_SRC_EVENT_DISPATCHER_H_
#define _EZNETPP_SRC_EVENT_DISPATCHER_H_

#include <vector>

#include "./macros.h"

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

  void push_event(const char* buffer, int len);

  // variables
private:
  std::vector<if_server*> server_container_;

private:
  DISALLOW_COPY_AND_ASSIGN(event_dispatcher);
};
  
}  // namespace eznetpp

#endif  // _EZNETPP_SRC_EVENT_DISPATCHER_H_
