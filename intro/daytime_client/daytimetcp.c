/*
 * Simple Daytime Client
 * This client establishes a TCP connection with a server that simply
 * sends back the current time and date in a human-readable format.
 *
 * Compile with:
 *   $ gcc -Wall -Werror -Wextra -pedantic daytimetcp.c -o daytimetcp
 *
 * Usage:
 *   $ ./daytimetcp
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
#define MAXLINE 64	/* max text line length */
#include <arpa/inet.h>	/* inet_pton */

int main()
{
	int	sockfd, n;
	char	recvline[MAXLINE + 1];
	char	*time_server;
	struct  sockaddr_in servaddr;

	time_server = "216.229.0.179";

	/* Create an Internet(AF_INET) stream(SOCK_STREAM) socket
	 * (that is, a TCP Socket)
	 * The socket function returns a file descriptor(sockfd)
	 * For more information about sockets: $ man 2 socket */
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	memset(&servaddr, 0, sizeof(servaddr)); /* Fill the struct with 0s) */
	servaddr.sin_family = AF_INET;		/* Address Family(Internet Protocol v4) */
	servaddr.sin_port   = htons(13);	/* Daytime server port(converted to MSB) */
	/* inet_pton function converts an address from text presentation(p) such as
	 * 192.168.1.1 to binary form(network(n) byte order)*/
	if (inet_pton(AF_INET, time_server, &servaddr.sin_addr) <= 0) {
		fprintf(stderr, "inet_pton error for %s\n", time_server);
		exit(1);
	}
	/* Connect the socket */
	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		perror("connect error");
	/* Read the response and prints it to stdout */
	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			perror("fputs error");
	}

	if (n < 0)
		perror("read error");

	return(0);
}
