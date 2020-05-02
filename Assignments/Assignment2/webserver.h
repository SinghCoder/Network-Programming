#ifndef WEBSERVER_H
#define WEBSERVER_H
#include "helper.h"
#include "hashtable.h"

void *processClients( void *input);
hashTable clientInfoTable;

#endif