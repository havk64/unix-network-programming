/*
 * Simple Daytime Client
 * This client establishes a TCP connection with a server that simply
 * sends back the current time and date in a human-readable format.
 * IPV6 Version
 *
 * Compile with:
 *   $ gcc -Wall -Werror -Wextra -pedantic daytimetcpv6.c -o daytimetcpv6
 *
 * Usage:
 *   $ ./daytimetcptcpv6 <ipv6 daytime server address>
 *
 * The output should be something like:
 *   $ 57793 17-02-09 22:47:41 00 0 0 167.9 UTC(NIST) *
 */

#include <stdio.h>      /* printf, perror */
#include <stdlib.h>     /* exit */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#define MAXLINE 51	/* max text line length */
#include <arpa/inet.h>	/* inet_pton */

int main(int argc, char ** argv)
{
	int	sockfd, n;
	char	recvline[MAXLINE + 1];
	struct  sockaddr_in6 servaddr;

	if(argc != 2) {
		fprintf(stderr, "usage: %s <IPaddress>", argv[0]);
		exit(1);
	}

	/* Create an Internet(AF_INET) stream(SOCK_STREAM) socket
	 * (that is, a TCP Socket)
	 * The socket function returns a file descriptor(sockfd)
	 * For more information about sockets: $ man 2 socket */
	if ( (sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	memset(&servaddr, 0, sizeof(servaddr)); /* Zero padding server address(servaddr) */
	servaddr.sin6_family = AF_INET6;	/* Internet Protocol v6 addresses */
	servaddr.sin6_port   = htons(13);	/* daytime server port */

	if (inet_pton(AF_INET6, argv[1], &servaddr.sin6_addr) <= 0) {
		fprintf(stderr, "inet_pton error for %s\n", argv[1]);
		exit(1);
	}

	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		perror("connect error");

	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			perror("fputs error");
	}

	if (n < 0)
		perror("read error");

	return(0);
}
