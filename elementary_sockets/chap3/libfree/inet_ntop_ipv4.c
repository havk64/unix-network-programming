#include <stdio.h>	/* snprintf */
#include <netinet/in.h> /* in_addr */
#include <string.h>     /* memcpy */
#include <errno.h>	/* errno */


const char * inet_ntop(int family, const void *addrptr, char *strptr, size_t len)
{
	const unsigned char *p = (const unsigned char *) addrptr;

	if (family == AF_INET) {
		char temp[INET_ADDRSTRLEN];
		snprintf(temp, sizeof(temp), "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
		if (strlen(temp) >= len) {
			errno = ENOSPC;
			return (NULL);
		}
		strcpy(strptr, temp);
		return (strptr);
	}
	errno = EAFNOSUPPORT;
	return(0);
}

