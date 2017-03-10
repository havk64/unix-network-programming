/* IPv6 socket address structure
 * As defined in the netinet/in.c
 */

struct in6_addr {
	uint8_t		s6_addr[16];	/* 128-bit IPv6 address (network byte ordered) */
};

#define SIN6_LEN			/* required for compile-time tests */

struct sockaddr_in6 {
	uint8_t		sin6_len;	/* length of the struct */
	sa_family_t	sin6_family;	/* AF_INET6 */
	in_port_t	sin6_port;	/* transport layer port# (network byte ordered) */
	uint32_t	sin6_flowinfo;	/* flow informatio, undefined */
	struct in6_addr sin6_addr;	/* IPv6 address (network byte ordered) */
	uint32_t	sin6_scope_id;	/* set of interfaces for a scope */
};
