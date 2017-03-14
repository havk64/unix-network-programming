#include <stdio.h>	/* snprintf */
#include <string.h>	/* strcat */
#include <netinet/in.h> /* in_addr */
#include <arpa/inet.h>	/* inet_ntop */

char * sock_ntop(const struct sockaddr *sa, socklen_t __attribute__((unused)) salen)
{
	char portstr[8];
	static char str[128]; /* Unix domain is largest */
	switch (sa->sa_family) {
		case AF_INET:{
			struct sockaddr_in *sin = (struct sockaddr_in *) sa;
			if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
				return(NULL);

			if(ntohs(sin->sin_port) != 0) {
       				snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
				strcat(str, portstr);
			}
			return(str);
		}
	}
	return(NULL);
}
