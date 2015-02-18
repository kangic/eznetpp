#ifndef _EZNETPP_SRC_CONNECTION_H_
#define _EZNETPP_SRC_CONNECTION_H_

#include "macros.h"

namespace eznetpp {
class connection {
public:
	connection();
	virtual ~connection();

public:
	int socket();

private:
	int sock_id;
};
 
} // namespace eznetpp

#endif	// _EZNETPP_SRC_CONNECTION_H_
