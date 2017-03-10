/* IPv6 new generic socket address structure
 * Defined as part of IPv6 sockets API
 */

struct sockaddr_storage {
	uint8_t		ss_len;		/* length of this struct (implementation dependent) */
	sa_family_t	ss_family;	/* address family: AF_xxx value - implementation-dependent elements to provide: */
	/* a) alignment sufficient to fulfill the alignment requirements of * all socket address types that the system supports.
	 * b) enough storage to hold any type of socket address that the system supports. */
};
