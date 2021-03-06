/* Simple Daytime server
 * Compile:
 *  $ gcc -Wall -Werror -Wextra -pedantic daytimetcpsrv.c -o daytimetcpsrv
 *
 * Usage:
 *  $ sudo ./daytimesrv (Bind might need root privileges)
 */

#include <stdio.h>      /* printf, perror */
#include <stdlib.h>     /* exit */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#define MAXLINE 51	/* max text line length */
#include <time.h>	/* time, ctime */
#include <errno.h>	/* errno */
#define LISTENQ 1024	/* queue size */

int main()
{
	int	listenfd, connfd;
	struct	sockaddr_in servaddr;
	char	buff[MAXLINE];
	time_t	ticks;
	/* Create the socket... */
	if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket error");
		exit(1);
	}
	/* Fill Internet socket address */
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(13);	/* daytime server */
	/* Bind server port to socket... */
	if( (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) ) < 0) {
		perror("bind error");
		exit(1);
	}
	/* Convert socket to listening socket */
	if( (listen(listenfd, LISTENQ)) < 0) {
		perror("listen error");
		exit(1);
	}
	printf("Listening on localhost...\n");
	/* Accept client connection, send reply... */
	for ( ; ; ) {
		if( (connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) < 0) {
			perror("accept error");
			exit(1);
		}

		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		write(connfd, buff, strlen(buff)); /* TO DO: error handling */

		close(connfd); /* TO DO: error handling */
	}
}
