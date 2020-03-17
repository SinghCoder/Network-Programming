#include "shm_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void err_exit(char *msg)
{
    printf("client : ");
    fflush(stdout);
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("USAGE: client <N> <M>\n");
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);

    union semun arg;
    int buffer_avail, tmp_buffer_avail;
    struct semid_ds ds;

    int semid = semget(CLI_SRVR_KEY, 3, 0666 | IPC_CREAT); /* 0: Reader, 1: Writer , 2: Amount of space in shared memory*/

    if(semid == -1){
        err_exit("semget failed:");
    }

    arg.val = 0;
    if( semctl(semid, 0, SETVAL, arg) == -1){    //number of readers = 0
        err_exit("semctl reader assignment: ");
    }

    arg.val = 1;
    if( semctl(semid, 1, SETVAL, arg) == -1){    //number of writers = 1
        err_exit("semctl writer assignment: ");
    }

    arg.val = M;
    if( semctl(semid, 2, SETVAL, arg) == -1){    //sizeof buffer available = M
        err_exit("semctl buffer assignment: ");
    }
    // arg.val = buffer_avail;
    if( (buffer_avail = semctl(semid, 2, GETVAL, arg)) == -1){        //get offset to write at
        err_exit("semctl get offset: ");
    }
    
    struct sembuf sops[1];
    struct data_item *data_ptr, *tmp_ptr;
    int shmid = shmget(SHM_KEY, M, 0666 | IPC_CREAT);
    if(shmid == -1){
        err_exit("shmget");
    }
    void* shm_addr = shmat(shmid, 0, 0);
    if(shm_addr == (void*)-1){
        err_exit("shmat: ");
    }

    for(int i = 0; i < N; i++){
        
        if( (tmp_buffer_avail = semctl(semid, 2, GETVAL, arg)) == -1){        //get offset to write at
            err_exit("semctl get offset: ");
        }

        arg.val = 0;

        sops[0].sem_num = 1;    
        sops[0].sem_op = -1;     //get access to write
        sops[0].sem_flg = 0;    

        if( semop(semid, sops, 1) == -1){
            err_exit("semop write access: ");
        }

        sops[0].sem_num = 2;    
        sops[0].sem_op = ((short)-1 * (short)sizeof(struct data_item));     //Check if there is space available in shared memory to write data item
        sops[0].sem_flg = 0;    
        if( semop(semid, sops, 1) == -1){
            err_exit("semop avail memory check: ");
        }

        if( (buffer_avail = semctl(semid, 2, GETVAL, arg)) == -1){        //get offset to write at
            err_exit("semctl get offset: ");
        }
        if(tmp_buffer_avail != 0)
            buffer_avail += sizeof(struct data_item);
        
        data_ptr = (struct data_item *)(shm_addr + M - buffer_avail);

        if(M-buffer_avail < 0){
            printf("Invalid location\n");
            exit(EXIT_FAILURE);
        }
        if(data_ptr == NULL){
            printf("Client, data_ptr is NULL\n");
        }
        data_ptr -> a = i+1;
        data_ptr -> b = i+1;
        data_ptr -> pid = getpid();
        data_ptr -> slno = i;

        printf("Client: pid - %d, slno - %d, a - %d, b - %d, shmid - %d, buffer space - %d \n", getpid(), data_ptr->slno, data_ptr->a, data_ptr->b, shmid, buffer_avail);
        sops[0].sem_num = 1;
        sops[0].sem_op = 1;     //leave write access
        sops[0].sem_flg = 0;    

        if( semop(semid, sops, 1) == -1){
            err_exit("semop leave write access: ");
        }
    }
    
    return 0;
}
