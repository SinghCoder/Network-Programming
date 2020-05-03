#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define SEMKEYPATH "/dev/null"  /* Path used on ftok for semget key  */
#define SEMKEYID 1              /* Id used on ftok for semget key    */
#define SHMKEYPATH "/dev/null"  /* Path used on ftok for shmget key  */
#define SHMKEYID 1              /* Id used on ftok for shmget key    */

#define NUMSEMS 2               /* Num of sems in created sem set    */
#define SIZEOFSHMSEG 50         /* Size of the shared mem segment    */


int main(int argc, char *argv[])
{
    int rc, semid, shmid, i;
    key_t semkey, shmkey;
    void *shm_address;
    struct sembuf operations[2];
    struct shmid_ds shmid_struct;
    short  sarray[NUMSEMS];

    
    
semkey = ftok(SEMKEYPATH,SEMKEYID);       
shmkey = ftok(SHMKEYPATH,SHMKEYID);
semid = semget( semkey, NUMSEMS, 0666 | IPC_CREAT | IPC_EXCL );

sarray[0] = 0;
sarray[1] = 0;
    
rc = semctl( semid, 1, SETALL, sarray);
    
shmid = shmget(shmkey, SIZEOFSHMSEG, 0666 | IPC_CREAT | IPC_EXCL);
shm_address = shmat(shmid, NULL, 0);

printf("Ready for client jobs\n");


        /* Set the structure passed into the semop() to first wait   */
        /* for the second semval to equal 1, then decrement it to    */
        /* allow the next signal that the client writes to it.       */
        /* Next, set the first semaphore to equal 1, which means     */
        /* that the shared memory segment is busy.                   */
        operations[0].sem_num = 1;/* Operate on the second sem     */
        operations[0].sem_op = -1;/* Decrement the semval by one   */
        operations[0].sem_flg = 0;/* Allow a wait to occur         */
        operations[1].sem_num = 0;/* Operate on the first sem      */
        operations[1].sem_op =  1;/* Increment the semval by 1     */
        operations[1].sem_flg = IPC_NOWAIT;/* Do not allow to wait    */
              
	rc = semop( semid, operations, 2 );
        
		/*reading from shared memory*/
			printf("Server Received : \"%s\"\n", (char *) shm_address);
		/*end of reading */

        /* Signal the first semaphore to free the shared memory.     */
        operations[0].sem_num = 0;
        operations[0].sem_op  = -1;
        operations[0].sem_flg = IPC_NOWAIT;
      
rc = semop( semid, operations, 1 );



rc = semctl( semid, 1, IPC_RMID );
    
rc = shmdt(shm_address);
    
rc = shmctl(shmid, IPC_RMID, &shmid_struct);

return 0;
}

