#include "TTalk/net/address.h"

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "unity.h"


void setUp(void) {}
void tearDown(void) {}

void test_addrinfo_is_family(void) {
	struct addrinfo addrinfo_tcpip4 = { .ai_family = AF_INET, .ai_socktype = SOCK_STREAM };
	struct addrinfo addrinfo_tcpip6 = { .ai_family = AF_INET6, .ai_socktype = SOCK_STREAM };
	struct addrinfo addrinfo_udpip4 = { .ai_family = AF_INET, .ai_socktype = SOCK_DGRAM };
	struct addrinfo addrinfo_udpip6 = { .ai_family = AF_INET6, .ai_socktype = SOCK_DGRAM };
	struct addrinfo addrinfo_unix  = { .ai_family = AF_UNIX };

	// TT_addrinfo_is_ip()
	TEST_ASSERT_TRUE(TT_addrinfo_is_ip(&addrinfo_tcpip4) && TT_addrinfo_is_ip(&addrinfo_tcpip6) &&
					 TT_addrinfo_is_ip(&addrinfo_udpip4) && TT_addrinfo_is_ip(&addrinfo_udpip6) &&
					 !TT_addrinfo_is_ip(&addrinfo_unix));

	// TT_addrinfo_is_tcp()
	TEST_ASSERT_TRUE(TT_addrinfo_is_tcp(&addrinfo_tcpip4) && TT_addrinfo_is_tcp(&addrinfo_tcpip6) &&
					 !TT_addrinfo_is_tcp(&addrinfo_udpip4) && !TT_addrinfo_is_tcp(&addrinfo_udpip6) &&
					 !TT_addrinfo_is_tcp(&addrinfo_unix));

	// TT_addrinfo_is_tcpip()
	TEST_ASSERT_TRUE(TT_addrinfo_is_tcpip(&addrinfo_tcpip4) && TT_addrinfo_is_tcpip(&addrinfo_tcpip6) &&
					 !TT_addrinfo_is_tcpip(&addrinfo_udpip4) && !TT_addrinfo_is_tcpip(&addrinfo_udpip6) &&
					 !TT_addrinfo_is_tcpip(&addrinfo_unix));
}

int main(void) {
	UNITY_BEGIN();

	RUN_TEST(test_addrinfo_is_family);

	return UNITY_END();
}

