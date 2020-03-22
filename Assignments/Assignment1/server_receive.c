#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>        // for strsep
#include <ctype.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdbool.h>

//#include "shell.h"
#define MAXPENDING 5
#define BUFFERSIZE 1000

int main() {
    /*CREATE A TCP SOCKET*/
    int serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket < 0) {
        perror("server socket creation");
        exit(1);
    }

    printf("Server Socket Created\n");
    
    /*CONSTRUCT LOCAL ADDRESS STRUCTURE*/
    struct sockaddr_in serverAddress, clientAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12349);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("Server address assigned\n");
    
    int temp = bind(serverSocket, (struct sockaddr * )&serverAddress, sizeof(serverAddress));
    
    if (temp < 0) {
        perror("binding");
        exit(1);
    }

    printf("Binding successful\n");
    
    int temp1 = listen(serverSocket, MAXPENDING);
    
    if (temp1 < 0) {
        perror("listen");
        exit(1);
    }

    printf("Now Listening\n");
    
    char msg[BUFFERSIZE];
    
    int clientLength = sizeof(clientAddress);
    for(;;){
        printf("SERVER trying to accept\n");
        int clientSocket = accept(serverSocket, (struct sockaddr * ) &clientAddress, &clientLength);
        
        if (clientLength < 0) {
            perror("client socket accept");
            exit(1);
        }

        printf("Sending data to client\n");
        char buffer[1000];
        memset(buffer , '\0' , 1000);
        strcpy(buffer , "hello what is up\nthis is a new line\nfor me and for you too\nthis is yet another line\n");
        int bytesSent = send(clientSocket, buffer, strlen(buffer), 0);
        printf("%s" , buffer);
        if(bytesSent == -1){
            perror("bytes sent");
            exit(1);
        }

        if (bytesSent != strlen(buffer)) {
            perror("sending message");
            exit(1);
        }
        while(1);
        // printf("ENTER MESSAGE FOR CLIENT\n");
        // gets(msg);
        
        // int bytesSent = send(clientSocket, msg, strlen(msg), 0);
        // if (bytesSent != strlen(msg)) {
        //     printf("Error while sending message to client");
        //     exit(0);
        // }
        
        // if( close(clientSocket) == -1)
        //     error_exit("client close");
        
        // printf("SERVER CLOSED SOCKET\n");
    }//end for
        
    close(serverSocket);
    return 0;
}