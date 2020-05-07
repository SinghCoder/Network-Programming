#ifndef SERVER_H
#define SERVER_H
#include "hashtable.h"
#include <signal.h>

#define BUFFERSIZE 1280
#define BACKLOG 10

/* Linked list functions */

clientNode *makeClient(int fd);

clientsInfo *initClients();

clientsInfo *insertClient(clientsInfo *clients, int fd, hashTable table);

clientsInfo *removeClient(clientsInfo *clients, int fd, hashTable table);

void printClients(clientsInfo *clients);

void sendToAllExceptSender(clientsInfo *clients, int fd, char *msg);
#endif