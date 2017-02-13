/*
 * Simple Daytime Client
 * This client establishes a TCP connection with a server and the server simply
 * sends back the current time and date in a human-readable format.

 * Compile with:
 *   $ gcc -Wall -Werror -Wextra -pedantic debug_msgs.c -o debug_msgs
 *
 * Usage:
 *   $ ./debug_msgs <address of any Internet Time Servers>
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
#define MAXLINE 51	/* max text line length */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>	/* inet_addr, inet_ntoa */
#include <errno.h>	/* errno */

int main(int argc, char **argv)
{
	int	sockfd, n, i;
	char	recvline[MAXLINE + 1];
	char	str[INET_ADDRSTRLEN];
	struct  sockaddr_in servaddr;
	struct  hostent *server;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <IPaddress>\n", argv[0]);
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

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(13);	/* daytime server */
	memcpy(&servaddr.sin_addr.s_addr, server->h_addr, server->h_length);

	inet_ntop(AF_INET, &(servaddr.sin_addr), str, INET_ADDRSTRLEN);
	/* Using inet_ntoa
	printf("h_addr is: %s\n", inet_ntoa(servaddr.sin_addr));*/

	/* Debug msgs */
	printf("host name: %7s%s\n", "", server->h_name);
	for(i = 0; (server->h_aliases)[i]; i++) {
			printf("host alias %i is:  %s\n", i+1, (server->h_aliases)[i]);
	}
	printf("host addr length: %d\n", server->h_length);
	for(i = 0; i < server->h_length; i++) {
		printf("byte %d: %10s0x%02x -> %*d\n", i+1, "", (unsigned char)(server->h_addr[i]), 3, (unsigned char)(server->h_addr[i]));
	}
	printf("h_addr is: %7s%s\n", "", str);
	printf("sin_addr is: %5s%u\n", "", servaddr.sin_addr.s_addr);
	printf("sin_addr is: %5s%x\n", "", servaddr.sin_addr.s_addr);
	/* End Debug */


	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		perror("connect error");

	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		printf("bytes read: %6s%d\n", "", n);
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			perror("fputs error");
	}

	if (n < 0)
		perror("read error");

	close(sockfd);
	return(0);
}
