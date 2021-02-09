#include "TTalk/net/socket.h"

#include <assert.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>

static bool addrinfo_is_ip(const struct addrinfo* const ai) {
	assert(ai);

	return ((ai->ai_protocol == AF_INET) || (ai->ai_protocol == AF_INET6));
}

static bool addrinfo_is_tcp(const struct addrinfo* const ai) {
	assert(ai);

	return (ai->ai_socktype == SOCK_STREAM);
}

static bool addrinfo_is_tcpip(const struct addrinfo* const ai) {
	assert(ai);

	return (addrinfo_is_ip(ai) && addrinfo_is_tcp(ai));
}

void TT_TCPSocket_init(struct TT_TCPSocket* const s) {
	assert(s);

	memset(s, 0, sizeof(*s));

	s->connected = false;
	s->fd = -1;
}

