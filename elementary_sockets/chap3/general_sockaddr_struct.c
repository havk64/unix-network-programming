/* Generic Socket address structure
 * as defined in the sys/socket.h header file
 */

struct sockaddr {
	uint8_t		sa_len;		/* length of structure */
	sa_family_t	sa_family;	/* address family: AF_xxx value */
	char		sa_data[14];	/* protocol-specific address */
};
