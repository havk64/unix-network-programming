/*
 * Simple Daytime Client
 * This client establishes a TCP connection with a server and the server simply
 * sends back the current time and date in a human-readable format.
 * It also prints the local address:port using sock_ntop function.
 *
 * Compile with:
 *   $ gcc -Wall -Werror -Wextra -pedantic daytimetcpcli.c
 *
 * Usage:
 *   $ ./daytime <address of any Internet Time Server in numerical form>
 *   e.g., 216.228.192.69
 * (Good list at: http://tf.nist.gov/tf-cgi/servers.cgi)
 *
 * The output should be something like:
 *   local addr: 192.168.1.2:53234
 *
 *   57793 17-02-09 22:47:41 00 0 0 167.9 UTC(NIST) *
 *   counter = 1
 */

#include <stdio.h>      /* printf, perror */
#include <stdlib.h>     /* exit */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#define MAXLINE 64	/* max text line length */
#include <arpa/inet.h>	/* inet_pton */

char *sock_ntop(const struct sockaddr *sa)
{
    char		portstr[8];
    static char str[128];		/* Unix domain is largest */
		struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

		if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
			return(NULL);
		if (ntohs(sin->sin_port) != 0) {
			snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
			strcat(str, portstr);
		}
		return(str);
}

int main(int argc, char **argv)
{
	int	sockfd, n, counter;
	socklen_t			len;
	char	recvline[MAXLINE + 1];
	char *str;
	struct  sockaddr_in servaddr, cliaddr;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <IPaddress>", argv[0]);
		exit(1);
	}
	/* Create an Internet(AF_INET) stream(SOCK_STREAM) socket
	 * (that is, a TCP Socket)
	 * The socket function returns a file descriptor(sockfd)
	 * For more information about sockets take a look at socket(2) man pages */
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	memset(&servaddr, 0, sizeof(servaddr)); /* Zero padding server address(servaddr) */
	servaddr.sin_family = AF_INET;		/* Internet Protocol v4 addresses */
	servaddr.sin_port   = htons(13);	/* daytime server port */
	/* inet_pton function converts an address in text presentation(p) such as
	 * '192.168.1.1' to binary form(network(n) byte order)*/
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		fprintf(stderr, "inet_pton error for %s\n", argv[1]);
		exit(1);
	}

	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		perror("connect error");

	len = sizeof(cliaddr);
	if( getsockname(sockfd, (struct sockaddr *)&cliaddr, &len) < 0) {
		fprintf(stderr, "getsockname error");
	}
	if ( (str = sock_ntop((struct sockaddr *) &cliaddr)) == NULL) {
		fprintf(stderr, "sock_ntop error");
	}
	printf("local addr: %s\n", str);

	counter = 0;
	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		counter++;
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			perror("fputs error");
	}
	if (n < 0)
		perror("read error");

	printf("counter = %d\n", counter);
	close(sockfd);
	return(0);
}
