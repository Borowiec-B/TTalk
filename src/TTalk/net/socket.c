#include "TTalk/net/socket.h"

#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>


void TT_TCPSocket_init(struct TT_TCPSocket* const s) {
	assert(s);

	memset(s, 0, sizeof(*s));

	s->connected = false;
	s->fd = -1;
}

