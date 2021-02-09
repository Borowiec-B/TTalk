#include "TTalk/net/socket.h"

#include <assert.h>
#include <string.h>


void TT_TCPSocket_init(struct TT_TCPSocket* const s) {
	assert(s);

	memset(s, 0, sizeof(*s));

	s->connected = false;
	s->fd = -1;
}

