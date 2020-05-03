#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

main(int argc,char **argv)
{
	int	sd;
	struct	sockaddr_in server;
	
	sd = socket (AF_INET,SOCK_STREAM,0);

	server.sin_family = AF_INET;
	//server.sin_addr.s_addr=inet_addr(argv[1]);
	//inet_aton(argv[1],&server.sin_addr);
	//inet_pton(AF_INET, argv[1], &server.sin_addr);
	server.sin_port = htons(12345);

	connect(sd, (struct sockaddr*) &server, sizeof(server));

        for (;;) {
	   send(sd, "HI", 2,0 );
           sleep(2);
        }
}
