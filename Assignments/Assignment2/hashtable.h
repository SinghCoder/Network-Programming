#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "helper.h"

#define PRIME 119
#define TABLE_SIZE 67
#define MAX_LEXEME_LEN 100
#define CLIENT_DNE NULL

typedef struct {
		int fd;
		clientInfo *cliInfo;
		bool present;
} hashElem;

typedef hashElem hashTable[TABLE_SIZE];

void initTable(hashTable table);

int hash(int fd);

clientInfo *searchTable(hashTable table, int fd);

void insertInTable(hashTable table, int fd, clientInfo *clInfo);
#endif