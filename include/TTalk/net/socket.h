#ifndef TT_SOCKET_H
#define TT_SOCKET_H

#include <stdbool.h>

#include "TTalk/net/address.h"


struct TT_TCPSocket {
	struct TT_Address local_address;
	struct TT_Address connected_address;

	bool connected;
	int fd;
};

void TT_TCPSocket_init(struct TT_TCPSocket* const s);


#endif

