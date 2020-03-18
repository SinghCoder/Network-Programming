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
    printf("inside shm.c\n\n");
    int shmid = atoi(argv[1]);
    int offset = atoi(argv[2]);
    int type = atoi(argv[3]);
    
    for(int i = 0; i < argc; i++){
        printf("argv[%d] : %s\n", i, argv[i]);  
    }

    struct shmid_ds shm_info;
    if( shmctl(shmid, IPC_STAT, &shm_info) == -1){
        error_exit("shmctl ");
    }

    void *shm_addr = shmat(shmid, NULL, 0);

    if(shm_addr == (void*)-1){
        error_exit("shmat ");
    }

    char *buffer = (char*)shm_addr;
    
    // if( ((buffer - (char*)shm_addr) * sizeof(char)) >= shm_info.shm_segsz){
    //     error_exit("shm full");
    // }
    FILE *fptr = fopen("tmp.txt", "w");

    if(type == SHM_WRITE){
            size_t sz;
            while((sz = fread(buffer, BUFFERSIZE, 1, stdin))){
                fprintf(stdout, "%s", buffer);
            }
        }
    else{
        // fprintf(stdout)
        // fileno()
        // printf("dakljsfkj;lasdkllllllllll\n");
        // fprintf(fptr, "=======asdahj,.hl;gjkwaehvjk====\n");
        printf("%s", buffer);
    }

    fclose(fptr);
   
   if(shmdt(shm_addr) == -1)   
        error_exit("shmdt");
    
    return 0;
}