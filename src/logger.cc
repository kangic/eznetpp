// Copyright[2015] <kangic21@gmail.com>

#include "../include/logger.h"

#include <cstdio>

namespace eznetpp {

logger* logger::inst;
std::mutex logger::log_mutex;

const char* log_level_str[] = {"FATAL", "ERROR", "WARN", "INFO", "DEBUG", };

logger::logger() {

}

logger::~logger() {

}

logger& logger::instance() {
	std::lock_guard<std::mutex> guard(log_mutex);
	if (inst == nullptr)
		inst = new logger();

	return *inst;
}

logger::cleanup::~cleanup() {
	std::lock_guard<std::mutex> guard(logger::log_mutex);
	if (logger::inst != nullptr) {
		delete logger::inst;
		logger::inst = nullptr;
	}
}

void logger::log(log_level level, const std::string& msg, const std::string& file
								, unsigned int line, const std::string& func) {
	std::lock_guard<std::mutex> guard(log_mutex);
	printf("[%s] %s", log_level_str[level], msg.c_str());

	if (file != "")
		printf("\t => occured at %s() - %d line in %s\n", func.c_str(), line, file.c_str());
}


} // namespace eznetpp
