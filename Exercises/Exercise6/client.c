#include "helper.h"

#define BUFFERSIZE 128

int main(void)
{
    int sockfd = 0;
    struct sockaddr_in serverAddr;

    /* Create a socket first */
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    /* Initialize sockaddr_in data structure */
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8004); // port
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Attempt a connection */
    if(connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
        errorExit("connect");
    }

    char buf[BUFFERSIZE];
    strcpy(buf, "hello server\n");
    Write(sockfd, buf, strlen(buf));    
    sleep(10);
    close(sockfd);
    return 0;
}
