// Copyright[2015] <kangic21@gmail.com>

#include "../include/runner.h"
#include "../include/logger.h"
#include "../include/if_server.h"

namespace eznetpp {

runner::runner(void) {
}

runner::~runner(void) {
}

void runner::register_server(if_server* server) {
  std::vector<if_server*>::iterator iter;

  std::lock_guard<std::mutex> guard(_server_container_mutex);
  for (iter = _server_container.begin();
       iter != _server_container.end();
       ++iter) {
    if (*iter == server) {
      // todo : print error log
    }
  }

  _server_container.push_back(server);

  /*
  bool ret = _evt_dispatcher.reg_server(listener);
  
  if (ret == false) {
    // todo : print error log
  }
  */
}

void runner::deregister_server(if_server* server) {
  std::vector<if_server*>::iterator iter;

  std::lock_guard<std::mutex> guard(_server_container_mutex);
  for (iter = _server_container.begin();
       iter != _server_container.end();
       ++iter) {
    if (*iter == server) {
      iter = _server_container.erase(iter);
    }
  }

  if (iter == _server_container.end()) {
    // todo : print error log
  }

  /*
  bool ret = _evt_dispatcher.dereg_server(listener);

  if (ret == false) {
    // todo : print error log
  }
  */
}

void runner::run(void) {
  logger::instance().log(logger::log_level::debug, "runner::run() ->\n");

  /*
  if (_evt_dispatcher.start_loop() == false) {
    logger::instance().log(logger::log_level::debug
        , "runner::run() <- evt_dispatcher.start_loop() failed..\n");
    return;
  }
  */

  std::vector<if_server*>::iterator iter;

  std::lock_guard<std::mutex> guard(_server_container_mutex);
  for (iter = _server_container.begin();
       iter != _server_container.end();
       ++iter) {
    if (0 != (*iter)->start_async_io())
      // todo : all stop to run the server already, print error log
      return;
  }

  while (true) {
  }
  logger::instance().log(logger::log_level::debug, "runner::run() <-\n");
}

}  // namespace eznetpp
