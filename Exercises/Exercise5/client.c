#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include "poll_n_server.h"
#define BUF_SIZE 100
int main (int argc, char **argv)
{
	int sd;
	char buf[BUF_SIZE];
	struct sockaddr_in server;

	snprintf(buf, BUF_SIZE, "Hello, My PID is %d", getpid());

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr ("127.0.0.1");
	//inet_aton(argv[1],&server.sin_addr);
	//inet_pton(AF_INET, argv[1], &server.sin_addr);
	server.sin_port = htons (SERVER_PORT);

	printf("%s\n", buf);
	for (;;)
	{
		sd = socket (AF_INET, SOCK_STREAM, 0);
		int ret = connect (sd, (struct sockaddr *) &server, sizeof (server));
		if (ret < 0)
		{
			perror ("connect");
			exit (-1);
		}
		printf ("client connection established\n");
			send (sd, buf, sizeof(buf), 0);
			int n;
		printf ("recieving data\n");
		while((n=recv(sd, buf, BUF_SIZE,0))>0){
			buf[n]='\0';
			printf("Output:\n%s", buf);
		}
			printf("Reciceived all data\n");
			//sleep (2);
	}
}