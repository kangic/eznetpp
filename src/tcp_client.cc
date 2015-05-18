//  Copyright [2015] <kangic21@gmail.com>

#include "../include/tcp_client.h"

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "../include/eznetpp.h"

#include "../include/tcp_connection.h"

namespace eznetpp {

tcp_client::tcp_client(void) {

}

tcp_client::~tcp_client(void) {

}

}  // namespace eznetpp
