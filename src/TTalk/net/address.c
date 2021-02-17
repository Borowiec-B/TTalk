#include "TTalk/net/address.h"

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netdb.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>


bool TT_addrinfo_is_ip(const struct addrinfo* const ai) {
	assert(ai);

	return ((ai->ai_family == AF_INET) || (ai->ai_family == AF_INET6));
}

bool TT_addrinfo_is_tcp(const struct addrinfo* const ai) {
	assert(ai);

	return (ai->ai_socktype == SOCK_STREAM);
}

bool TT_addrinfo_is_tcpip(const struct addrinfo* const ai) {
	assert(ai);

	return (TT_addrinfo_is_ip(ai) && TT_addrinfo_is_tcp(ai));
}

TT_Status TT_ip_family_str_length(const int family, size_t* const ret) {
	assert(ret);

	if (family == AF_INET) {
		*ret = 16;

		return TT_STATUS_SUCCESS;
	}
	else if (family == AF_INET6) {
		*ret = INET6_ADDRSTRLEN;

		return TT_STATUS_SUCCESS;
	} else {
		return TT_STATUS_INVALID_ARG;
	}
}

struct addrinfo* TT_find_first_tcpip_addrinfo(const struct addrinfo* const node) {
	assert(node);

	const struct addrinfo* checked_addrinfo = &node[0];

	while (!TT_addrinfo_is_tcpip(checked_addrinfo)) {
		if (!checked_addrinfo->ai_next)
			return NULL;

		checked_addrinfo = checked_addrinfo->ai_next;
	}

	return checked_addrinfo;
}

TT_Status TT_sockaddr_ip_to_str(const struct sockaddr* const sa, char* buf, const socklen_t buf_size) {
	assert(sa && buf);

	if (sa->sa_family == AF_INET) {
		struct sockaddr_in* sin = (struct sockaddr_in*)sa;

		if (!inet_ntop(AF_INET, &sin->sin_addr.s_addr, buf, buf_size))
			goto handle_ntop_error;

		return TT_STATUS_SUCCESS;
	} else if (sa->sa_family == AF_INET6) {
		struct sockaddr_in6* sin6 = (struct sockaddr_in6*)sa;

		if (!inet_ntop(AF_INET6, &sin6->sin6_addr, buf, buf_size))
			goto handle_ntop_error;

		return TT_STATUS_SUCCESS;
	} else {
		// sa->family is invalid.
		return TT_STATUS_INVALID_ARG;
	}

handle_ntop_error:
	if (errno == ENOSPC)
		return TT_STATUS_BUFFER_OVERFLOW;
	else
		return TT_STATUS_ERROR;
}

TT_Status TT_Address_init_from_hostname(struct TT_Address* const ret, const char* const hostname, const char* const port) {
	assert(ret && hostname && port);

	const struct addrinfo hints = {
		.ai_family = AF_UNSPEC,
		.ai_socktype = SOCK_STREAM
	};

	struct addrinfo* results;

	int status = getaddrinfo(hostname, port, &hints, &results);
	if (status != 0) {	
		switch (status) {
			case EAI_AGAIN:
				return TT_STATUS_TEMPORARY_FAILURE;
				break;
			case EAI_MEMORY:
				return TT_STATUS_OUT_OF_MEMORY;
				break;
			default:
				return TT_STATUS_ERROR;
				break;
		}
	}
	
	struct addrinfo* found_addrinfo = TT_find_first_tcpip_addrinfo(results);
	if (!found_addrinfo)
		return TT_STATUS_ERROR;


	memset(ret, 0, sizeof(*ret));

	struct sockaddr* ret_sa = (struct sockaddr*)&ret->ss;
	*ret_sa = *found_addrinfo->ai_addr;

	size_t ip_buf_size;
	ret->ip = malloc(TT_ip_family_str_length(ret_sa->sa_family, &ip_buf_size));
	if (!ret->ip)
		return TT_STATUS_OUT_OF_MEMORY;
	TT_sockaddr_ip_to_str(ret_sa, ret->ip, ip_buf_size);

	ret->port = strdup(port);
	if (!ret->port)
		return TT_STATUS_OUT_OF_MEMORY;

	return TT_STATUS_SUCCESS;
}

