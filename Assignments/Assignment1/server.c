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
    serverAddress.sin_port = htons(12345);
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

        printf("Received from client\n");
        while(1)
        {
            printf("server waiting for data\n");
            memset(msg, '\0' , BUFFERSIZE);
            int temp2 = recv(clientSocket, msg, BUFFERSIZE, 0);
            printf("temp2 : %d\n", temp2);
            if (temp2 < 0) {
                perror("recv data from client");
                exit(1);
            }
            if(!strcmp(msg, "exit\n")){
                close(clientSocket);
                break;
            }

            printf("%s\n", msg);
        }
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
    }
        
    close(serverSocket);
    return 0;
}