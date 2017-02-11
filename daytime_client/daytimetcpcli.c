/*
 * Simple Daytime Client
 * This client establishes a TCP connection with a server and the server simply
 * sends back the current time and date in a human-readable format.
 *
 * Compile with:
 *   $ gcc -Wall -Werror -Wextra -pedantic daytimetcpcli.c -o daytime
 *
 * Usage:
 *   $ ./daytime <address of any Internet Time Server>
 * (Here you have a good list: http://tf.nist.gov/tf-cgi/servers.cgi)
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
#define MAXLINE 4096	/* max text line length */
#include <netdb.h>      /* struct hostent, gethostbyname */

int main(int argc, char **argv)
{
	int	sockfd, n;
	char	recvline[MAXLINE + 1];
	struct  sockaddr_in servaddr;
	struct  hostent *server;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <IPaddress>", argv[0]);
		exit(1);
	}

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	if ( (server = gethostbyname(argv[1])) == NULL ) {
		perror("Error, no such host!");
		exit(1);
	}

	memset(&servaddr, 0, sizeof(servaddr)); /* Zero padding server address(servaddr) */
	servaddr.sin_family = AF_INET;		/* Internet Protocol v4 addresses */
	servaddr.sin_port   = htons(13);	/* daytime server */
	memcpy(&servaddr.sin_addr.s_addr, server -> h_addr, server -> h_length);

	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		perror("connect error");

	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			perror("fputs error");
	}

	if (n < 0)
		perror("read error");

	close(sockfd);
	return(0);
}
