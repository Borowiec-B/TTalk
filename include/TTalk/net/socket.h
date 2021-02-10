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

bool addrinfo_is_ip(const struct addrinfo* const ai);
bool addrinfo_is_tcp(const struct addrinfo* const ai);
bool addrinfo_is_tcpip(const struct addrinfo* const ai);
struct addrinfo* find_first_tcpip_addrinfo(const struct addrinfo* const linked_list);

void TT_TCPSocket_init(struct TT_TCPSocket* const s);


#endif

