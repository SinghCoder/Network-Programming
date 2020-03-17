#ifndef SHM_HEADER_H
#define SHM_HEADER_H
#include <sys/types.h>

#define CLI_SRVR_KEY 0x13061
#define SHM_KEY 0x13411
#define MAX_TRIES 10

struct data_item{
    pid_t pid;      // client pid
    int slno;       // incremented for every item
    int a;          // any number
    int b;          // any number
};

union semun {
       int              val;    /* Value for SETVAL */
       struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
       unsigned short  *array;  /* Array for GETALL, SETALL */
       struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
};

#endif