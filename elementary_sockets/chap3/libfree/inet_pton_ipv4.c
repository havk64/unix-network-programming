/* Converts a dot decimal notation string with an IPv4 address to a binary form.
 * "p" stands for presentation and "n" to numeric("p" to "n" => presentation to numeric)
 */
#include <netinet/in.h> /* in_addr */
#include <string.h>     /* memcpy */
#include <errno.h>	/* errno */
#include <arpa/inet.h>	/* inet_aton */

int inet_pton(int family, const char *strptr, void *addrptr)
{
	if (family == AF_INET) {
		struct in_addr in_val;
		if (inet_aton(strptr, &in_val)) {
			memcpy(addrptr, &in_val, sizeof(struct in_addr));
			return(1);
		}
		return(0);
	}
	errno = EAFNOSUPPORT;
	return(-1);
}
