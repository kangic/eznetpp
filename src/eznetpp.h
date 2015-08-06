// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_EZNETPP_H_
#define INCLUDE_EZNETPP_H_

// c headers
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>

// c++ headers
#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// eznetpp headers
#include "common.h"
#include "macros.h"
#include "util/logger.h"


#endif  // INCLUDE_EZNETPP_H_
