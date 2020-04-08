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
int main (int argc, char **argv)
{
	int sd;
	char buf[MAX_BFR_SIZE];
	struct sockaddr_in server;

	snprintf(buf, MAX_BFR_SIZE, "Hello, My PID is %d", getpid());

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr ("127.0.0.1");
	server.sin_port = htons (SERVER_PORT);

	printf("Data sent by me : %s\n", buf);
	sd = socket (AF_INET, SOCK_STREAM, 0);
	
	if(sd < 0){
		perror("socket");
		exit(EXIT_FAILURE);
	}

	int ret = connect (sd, (struct sockaddr *) &server, sizeof (server));
	if (ret < 0)
	{
		perror ("connect");
		exit(EXIT_FAILURE);
	}
	printf ("client connection established\n");

	for (;;)
	{		
		send (sd, buf, sizeof(buf), 0);
		int n;
		printf ("recieving data\n");
		n = recv(sd, buf, MAX_BFR_SIZE,0);
		if(n < 0){
			perror ("recv");
			exit(EXIT_FAILURE);
		}

		while(n > 0){			
			buf[n]='\0';
			printf("Data recieved by me: %s\n", buf);
			n = recv(sd, buf, MAX_BFR_SIZE,0);
			if(n < 0){
				perror ("recv");
				exit(EXIT_FAILURE);
			}
		}
	}
}