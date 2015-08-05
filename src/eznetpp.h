// Copyright[2015] <kangic21@gmail.com>

#ifndef INCLUDE_EZNETPP_H_
#define INCLUDE_EZNETPP_H_

// c headers
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>

// c++ headers
#include <algorithm>
#include <atomic>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

// eznetpp headers
#include "common.h"
#include "macros.h"
#include "util/logger.h"


#endif  // INCLUDE_EZNETPP_H_
