// Copyright[2015] <kangic21@gmail.com>

#include "../include/runner.h"
#include "../include/logger.h"

namespace eznetpp {

runner::runner(void) {
}

runner::~runner(void) {
}

void runner::register_server(if_server* listener) {
  bool ret = evt_dispatcher.reg_server(listener);

  if (ret == false) {
    // todo : print error log
  }
}

void runner::deregister_server(if_server* listener) {
  bool ret = evt_dispatcher.dereg_server(listener);

  if (ret == false) {
    // todo : print error log
  }
}

void runner::run(void) {
  logger::instance().log(logger::log_level::debug, "runner::run() ->\n");
  if (evt_dispatcher.start_loop() == false) {
    logger::instance().log(logger::log_level::debug
        , "runner::run() <- evt_dispatcher.start_loop() failed..\n");
    return;
  }

  while (true) {
  }
  logger::instance().log(logger::log_level::debug, "runner::run() <-\n");
}

}  // namespace eznetpp
