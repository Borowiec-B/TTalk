#ifndef TT_ADDRESS_H
#define TT_ADDRESS_H

#include <stdbool.h>
#include <sys/socket.h>

#include "TTalk/status.h"


struct addrinfo;

struct TT_Address {
	struct sockaddr_storage ss;
	const char* ip;
	const char* port;
};

bool TT_addrinfo_is_ip(const struct addrinfo* const ai);
bool TT_addrinfo_is_tcp(const struct addrinfo* const ai);
bool TT_addrinfo_is_tcpip(const struct addrinfo* const ai);

/*
 *  TT_find_first_tcpip_addrinfo(): Return pointer to first found node with a TCP/IP sockaddr, or NULL if not found.
 */
struct addrinfo* TT_find_first_tcpip_addrinfo(const struct addrinfo* const node);
TT_Status TT_sockaddr_ip_to_str(const struct sockaddr* const sa, char* buf, const socklen_t buf_size);

#endif
