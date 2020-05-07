#include "hashtable.h"

int hash(int fd){
    return fd % TABLE_SIZE;
}
/**
 * @brief initialize a hash table
 */
void initTable(hashTable table) {
	
	if (table == NULL) {
		errorExit("HASH table allocation error");
	}
	for (int i = 0; i < TABLE_SIZE; i++) {
		table[i].present = false;
		table[i].cliInfo = NULL; 
	}
}

void insertInTable(hashTable table, int fd, clientNode *clInfo){
	// printf("Inserting entry for %d in table\n", fd);
	int hashValue;
	hashValue = hash(fd);
	// printf("hash is %d\n", hashValue);
	// printf("is cliInfo null? %s\n", (clInfo == NULL) ? "Yeah" : "Hell no" );
	int probeNum = 1;

	while (table[hashValue].present == true) {
		if(table[hashValue].fd == fd)	//updating a key's value instead inserting a new one
			break;
		hashValue = (hashValue + probeNum * probeNum) % TABLE_SIZE;
		probeNum++;
	}	

	table[hashValue].present = true;
	table[hashValue].fd = fd;
	table[hashValue].cliInfo = clInfo;
}

/**
 * @brief search a string in the hash table
 * 
 * @return int : hash value else -1 if fd not found
 */

clientNode *searchTable(hashTable table, int fd) {
	// printf("Searching in hashtable for %d\n", fd);
	int hashValue = hash(fd);
	// printf("hash is %d\n", hashValue);
	int probeNum = 1;
	while (table[hashValue].present == true) 
	{
		if (table[hashValue].fd == fd) {
			return table[hashValue].cliInfo;
		}
		hashValue = (hashValue + probeNum * probeNum) % TABLE_SIZE;
		probeNum++;
	}
	return CLIENT_DNE;
}

void removeFromTable(hashTable table, int fd){
	int hashValue = hash(fd);
	int probeNum = 1;
	while (table[hashValue].present == true) 
	{
		if (table[hashValue].fd == fd) {
			table[hashValue].present = false;
			return;
		}
		hashValue = (hashValue + probeNum * probeNum) % TABLE_SIZE;
		probeNum++;
	}
}