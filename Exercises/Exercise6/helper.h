#ifndef HELPER_H
#define HELPER_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for read() / write()
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SUCCESS 1

/* Client info */
typedef enum {false, true} bool;

typedef struct clientNode{
    int fd;
    bool helloSent;
    bool helloRcvd;
    struct clientNode *nextClient;
    struct clientNode *prevClient;  /* stored to perform deletion in O(1)*/
} clientNode;

typedef struct clientsInfo{
    int numClients;
    clientNode *firstClient;
    clientNode *lastClient;
} clientsInfo;

/* Exit on system call error function*/
void errorExit(char *msg);

void errorExitPthread(char *msg, int errnum);

/* Socket functions */

int Socket(int domain, int type, int protocol);

int Listen(int sockfd, int backlog);

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int Open(const char *pathname, int flags);

ssize_t Read(int fd, void *buf, size_t count);

ssize_t Write(int fd, const void *buf, size_t count);

int Close(int fd);

char *getTimestamp();

#endif