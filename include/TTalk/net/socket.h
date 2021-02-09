#ifndef TT_SOCKET_H
#define TT_SOCKET_H

#include <netdb.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>


struct TT_Address {
	struct sockaddr_storage ss;
	const char* ip;
	const char* port;
};

struct TT_TCPSocket {
	struct TT_Address local_address;
	struct TT_Address connected_address;

	bool connected;
	int fd;
};


#endif

