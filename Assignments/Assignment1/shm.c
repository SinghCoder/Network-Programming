#include "shell.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>        // for strsep
#include <ctype.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/shm.h>

int main(int argc, char *argv[])
{
    int shmid = atoi(argv[1]);  // id of shared memory where r/w to be performed
    int type = atoi(argv[2]);   // operation type SHM_WRITE / SHM_READ
    
    void *shm_addr = shmat(shmid, NULL, 0); //attach shared mem

    if(shm_addr == (void*)-1){
        error_exit("shmat ");
    }

    int offset = *(int *)shm_addr;          // offset to write at is stored in first integer location of shared mem

    char *buffer;
    size_t size = BUFFERSIZE;
    if(type == SHM_WRITE){
        buffer = (char*)( shm_addr + sizeof(int) + offset);   // write after offset
        size_t sz;

        while((sz = getline(&buffer, &size, stdin)) != -1){
            
            *(int *)shm_addr += sz;     // update offset value

            fprintf(stdout, "%s", buffer);
            buffer = buffer + sz;       // go to end of data
        }

    }
    else{
        buffer = (char*)(shm_addr + sizeof(int));
        printf("%s", buffer);
    }
   
    if(shmdt(shm_addr) == -1)   
        error_exit("shmdt");
    
    return 0;
}