#include "TTalk/net/address.h"

#include <assert.h>
#include <netdb.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/types.h>


bool addrinfo_is_ip(const struct addrinfo* const ai) {
	assert(ai);

	return ((ai->ai_family == AF_INET) || (ai->ai_family == AF_INET6));
}

bool addrinfo_is_tcp(const struct addrinfo* const ai) {
	assert(ai);

	return (ai->ai_socktype == SOCK_STREAM);
}

bool addrinfo_is_tcpip(const struct addrinfo* const ai) {
	assert(ai);

	return (addrinfo_is_ip(ai) && addrinfo_is_tcp(ai));
}

struct addrinfo* find_first_tcpip_addrinfo(const struct addrinfo* const linked_list) {
	assert(linked_list);

	const struct addrinfo* checked_addrinfo = &linked_list[0];

	while (!addrinfo_is_tcpip(checked_addrinfo)) {
		if (!checked_addrinfo->ai_next)
			return NULL;

		checked_addrinfo = checked_addrinfo->ai_next;
	}

	return checked_addrinfo;
}
