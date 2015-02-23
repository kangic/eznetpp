#ifndef _EZNETPP_SRC_DUMMY_CONNECTION_H_
#define _EZNETPP_SRC_DUMMY_CONNECTION_H_

#include "macros.h"

namespace eznetpp {
class dummy_connection {
public:
	dummy_connection();
	virtual ~dummy_connection();

public:
	void socket(int id);
	int socket();

private:
	int sock_id;
};
 
} // namespace eznetpp

#endif	// _EZNETPP_SRC_DUMMY_CONNECTION_H_
