// Copyright[2015] <kangic21@gmail.com>

#include "../include/runner.h"

#include <cstdio>

namespace eznetpp {

runner::runner(void) {
}

runner::~runner(void) {
}

void runner::register_event_listener(if_event_listener* listener) {
	bool ret = evt_dispatcher.reg_event_listener(listener);

	if (ret == false) {
		// todo : print error log
	}
}

void runner::deregister_event_listener(if_event_listener* listener) {
	bool ret = evt_dispatcher.dereg_event_listener(listener);

	if (ret == false) {
		// todo : print error log
	}
}

void runner::run(void) {
	// step 1. 
	
}

}  // namespace eznetpp
