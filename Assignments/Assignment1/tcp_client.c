#include <stdio.h>
#include <sys/socket.h> //for socket(), connect(), send(), recv() functions
#include <arpa/inet.h> // different address structures are declared here
#include <stdlib.h> // atoi() which convert string to integer
#include <string.h>
#include <unistd.h> // close() function
#include <errno.h>
#include <signal.h>
#include "shell.h"

#define BUFFERSIZE 1000
int main(int argc , char*argv[])
{
	int sockfd = atoi(argv[1]);
    int cmd = atoi(argv[2]);
	
    if(cmd == SOCK_WRITE)
    {
		char *buffer = (char*)malloc(sizeof(char) * (BUFFERSIZE+1) );
		memset(buffer, '\0', BUFFERSIZE+1);
	    char c = fgetc(stdin); 
	    int i = 0;
	    while (c != EOF) 
	    { 
	        buffer[i] = c; 
	        c = fgetc(stdin);
	        i++; 
	    }//end while
	    printf("Sending : \n");
	    printf("%s\n" , buffer );

	    int bytesSent = send(sockfd, buffer, strlen(buffer), 0);
		if(bytesSent == -1){
	        printf("Error : %s", strerror(errno) ) ;
	    }

	    if (bytesSent != strlen(buffer)) {
	        perror("sending message");
	        exit(1);
	    }
	}//end if sockwrite
	else
	{
		char recvBuffer[BUFFERSIZE];
        int bytesRecvd = recv(sockfd, recvBuffer, BUFFERSIZE, 0);
        if (bytesRecvd < 0) {
            perror("Receiving data from server");
        	exit(1);
        }
        printf("%s", recvBuffer);
	}//end else
 
  
	exit(0);
   return 0;


}//end main