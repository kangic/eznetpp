#include "../include/event_dispatcher.h"
#include "../include/logger.h"

#include "../include/if_server.h"

namespace eznetpp {

event_dispatcher::event_dispatcher(void) {
  server_container_.clear();
}

event_dispatcher::~event_dispatcher(void) {
  server_container_.clear();
}

bool event_dispatcher::reg_server(
    if_server* server) {
  // step 1. check to exist object.
  std::vector<if_server*>::iterator iter;
  for (iter = server_container_.begin();
       iter != server_container_.end();
       ++iter) {
    if (*iter == server)
      return false;
  }

  // step 2. add server to the server_container
  server_container_.push_back(server);

  return true;
}

bool event_dispatcher::dereg_server(
    if_server* server) {
  std::vector<if_server*>::iterator iter;
  for (iter = server_container_.begin();
       iter != server_container_.end();
       ++iter) {
    if (*iter == server) {
      iter = server_container_.erase(iter);
      return true;
    }
  }

  return false;
}

bool event_dispatcher::start_loop() {
  std::vector<if_server*>::iterator iter;
  for (iter = server_container_.begin();
       iter != server_container_.end();
       ++iter) {
		if( 0 != (*iter)->start_async_io() )
			return false;
	}

	return true;
}

}  // namespace eznetpp
