#include "shm_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

int semid;
int shmid;
void* shm_addr;
void err_exit(char *msg)
{
    printf("server : ");
    perror(msg);
    exit(EXIT_FAILURE);
}

void cleanup(int signo)
{
    if( shmdt(shm_addr) == -1){
        err_exit("shmdt: ");
    }
    if( semctl(semid, 0, IPC_RMID) == -1){
        err_exit("sem rm");
    }
    if( shmctl(shmid, IPC_RMID, NULL) == -1){
        err_exit("shm rm");
    }
}

int main(int argc, char* argv[])
{

    //apply signal handler for cleanup on SIGINT
    signal(SIGINT, &cleanup);

    if(argc < 2)
    {
        printf("USAGE: server <M>\n");
        exit(EXIT_FAILURE);
    }

    int M = atoi(argv[1]);

    union semun arg;
    int buffer_avail;
    struct semid_ds ds;

    semid = semget(CLI_SRVR_KEY, 3, 0666 | IPC_CREAT); /* 0: Reader, 1: Writer , 2: Amount of space in shared memory*/

    if(semid == -1){
        err_exit("semget failed:");
    }

    struct sembuf sops[1];
    struct data_item *data_ptr;
    shmid = shmget(SHM_KEY, M, 0666 | IPC_CREAT);
    
    if(shmid == -1){
        err_exit("shmget");
    }

    shm_addr = shmat(shmid, NULL, 0);
    
    if(shm_addr == (void*)-1){
        err_exit("shmat: ");
    }

    int offset = 0;

    // make sure semaphores are initialized

    for(int j = 0; j < MAX_TRIES; j++){
        arg.buf = &ds;
        if(semctl(semid, 0 , IPC_STAT, arg) == -1){
            perror("semctl: ");
            exit(EXIT_FAILURE);
        }
        if(ds.sem_otime != 0)
            break;
        sleep(1);
    }

    if(ds.sem_otime == 0){
        printf("Existing semaphore not initialized\n");
        exit(EXIT_FAILURE);
    }

    for(;;){

        if( (buffer_avail = semctl(semid, 2, GETVAL, arg) ) == -1){
            err_exit("semctl get offset: ");
        }     

        if( M - buffer_avail >= sizeof(struct data_item)){

            data_ptr = (struct data_item*)(shm_addr + offset);
            offset += sizeof(struct data_item);
            offset %= M;
            printf("Server: pid - %d, slno - %d, a - %d, b - %d, a+b - %d, shmid - %d, buffer space - %d \n", getpid(), data_ptr->slno, data_ptr->a, data_ptr->b, data_ptr->a + data_ptr->b , shmid, buffer_avail);

            sops[0].sem_num = 2;    
            sops[0].sem_op = sizeof(struct data_item);     //increase size of free memory
            sops[0].sem_flg = 0;  
            
            if( semop(semid, sops, 1) == -1){
                err_exit("semop pick data from shmem: ");
            }
            
            if( (buffer_avail = semctl(semid, 2, GETVAL, arg) ) == -1){
                err_exit("semctl get offset: ");
            } 
            
            if( M - buffer_avail == 0){
                offset = 0;
            }
        }
        
    }
    
    return 0;
}
