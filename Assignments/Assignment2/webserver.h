#ifndef WEBSERVER_H
#define WEBSERVER_H
#include "helper.h"
#include "hashtable.h"
#include <netinet/in.h>
#include <arpa/inet.h>

#define NUMTHREADS 5
#define MAX_QUEUE_SIZE 10

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8004

void *processClients( void *input);
hashTable clientInfoTable;

#endif