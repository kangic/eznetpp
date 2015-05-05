// Copyright [2015] <kangic21@gmail.com>

#include "../include/event_dispatcher.h"
#include "../include/logger.h"
#include "../include/if_server.h"

namespace eznetpp {

event_dispatcher::event_dispatcher(void) {
  _server_container.clear();
}

event_dispatcher::~event_dispatcher(void) {
  _server_container.clear();
}

bool event_dispatcher::reg_server(if_server* server) {
  std::vector<if_server*>::iterator iter;

  std::lock_guard<std::mutex> guard(_server_container_mutex);
  for (iter = _server_container.begin();
       iter != _server_container.end();
       ++iter) {
    if (*iter == server)
      return false;
  }

  _server_container.push_back(server);

  return true;
}

bool event_dispatcher::dereg_server(if_server* server) {
  std::vector<if_server*>::iterator iter;

  std::lock_guard<std::mutex> guard(_server_container_mutex);
  for (iter = _server_container.begin();
       iter != _server_container.end();
       ++iter) {
    if (*iter == server) {
      iter = _server_container.erase(iter);
      return true;
    }
  }

  return false;
}

bool event_dispatcher::start_loop() {
  std::vector<if_server*>::iterator iter;

  std::lock_guard<std::mutex> guard(_server_container_mutex);
  for (iter = _server_container.begin();
       iter != _server_container.end();
       ++iter) {
    if (0 != (*iter)->start_async_io())
      return false;
  }

  return true;
}

}  // namespace eznetpp
