#include "TTalk/net/address.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
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

void test_find_first_tcpip_addrinfo(void) {
	struct addrinfo fourth_node = { .ai_family = AF_UNIX };
	struct addrinfo third_node = { .ai_family = AF_INET6, .ai_socktype = SOCK_STREAM, .ai_next = &fourth_node };
	struct addrinfo second_node = { .ai_family = AF_INET, .ai_socktype = SOCK_DGRAM, .ai_next = &third_node };
	struct addrinfo first_node = { .ai_family = AF_UNIX, .ai_socktype = SOCK_STREAM, .ai_next = &second_node };

	TEST_ASSERT_TRUE(TT_find_first_tcpip_addrinfo(&first_node) == &third_node);
}

void test_sockaddr_ip_to_str(void) {
	struct sockaddr_in  sin  = { .sin_family = AF_INET };
	struct sockaddr_in6 sin6 = { .sin6_family = AF_INET6 };

	// Set sin's and sin6's addresses to network representation.
	const char* const ipv4_address = "127.1.2.3";
	const char* const ipv6_address = "2001:0db8:85a3:0000:0000:8a2e:0370:7334";
	inet_pton(AF_INET, ipv4_address, &sin.sin_addr.s_addr);
	inet_pton(AF_INET6, ipv6_address, &sin6.sin6_addr);

	// TT_sockaddr_ip_to_str() should return strings equal to what inet_ntop() would write.
	// So, use inet_ntop() to create expected values.
	char ipv4_expected_str_buf[INET6_ADDRSTRLEN] = { 0 };
	char ipv6_expected_str_buf[INET6_ADDRSTRLEN] = { 0 };
	inet_ntop(AF_INET, &sin.sin_addr.s_addr, ipv4_expected_str_buf, sizeof(ipv4_expected_str_buf));
	inet_ntop(AF_INET6, &sin6.sin6_addr, ipv6_expected_str_buf, sizeof(ipv6_expected_str_buf));

	// Test actual strings.
	char ipv4_actual_str_buf[INET6_ADDRSTRLEN] = { 0 };
	char ipv6_actual_str_buf[INET6_ADDRSTRLEN] = { 0 };

	TEST_ASSERT_TRUE(TT_sockaddr_ip_to_str((struct sockaddr*)&sin, ipv4_actual_str_buf, sizeof(ipv4_actual_str_buf)) == TT_STATUS_SUCCESS);
	TEST_ASSERT_EQUAL_STRING(ipv4_expected_str_buf, ipv4_actual_str_buf);

	TEST_ASSERT_TRUE(TT_sockaddr_ip_to_str((struct sockaddr*)&sin6, ipv6_actual_str_buf, sizeof(ipv6_actual_str_buf)) == TT_STATUS_SUCCESS);
	TEST_ASSERT_EQUAL_STRING(ipv6_expected_str_buf, ipv6_actual_str_buf);

	// Test statuses returned when supplying invalid arguments.
	struct sockaddr	invalid_family_sockaddr = { .sa_family = AF_UNIX };
	char not_big_enough_buf[2] = { 0 };

	// Invalid sockaddr's family should return TT_STATUS_INVALID_ARG.
	TEST_ASSERT_TRUE(TT_sockaddr_ip_to_str((struct sockaddr*)&invalid_family_sockaddr, ipv6_actual_str_buf, sizeof(ipv6_actual_str_buf)) == TT_STATUS_INVALID_ARG);
	// Supplied buf_size being not large enough should return TT_STATUS_BUFFER_OVERFLOW.
	TEST_ASSERT_TRUE(TT_sockaddr_ip_to_str((struct sockaddr*)&sin6, not_big_enough_buf, sizeof(not_big_enough_buf)) == TT_STATUS_BUFFER_OVERFLOW);
}

int main(void) {
	UNITY_BEGIN();

	RUN_TEST(test_addrinfo_is_family);
	RUN_TEST(test_find_first_tcpip_addrinfo);
	RUN_TEST(test_sockaddr_ip_to_str);

	return UNITY_END();
}

