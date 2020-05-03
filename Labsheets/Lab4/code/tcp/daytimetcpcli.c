#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define MAXLINE 80



typedef struct sockaddr SA;

int
main(int argc, char **argv)
{
	int	sockfd, n;
	char	recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;

	if (argc != 2)
		{
		printf("usage: a.out <IPaddress>\n");
		exit(1);
		}

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		perror("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(25);	/* daytime server */
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		printf("inet_pton error\n");

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		perror("connect error");

printf("Connection Established");

	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			perror("fputs error");
	}
	if (n < 0)
		perror("read error");

	exit(0);
}
