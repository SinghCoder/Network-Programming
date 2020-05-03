#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define SEMKEYPATH "/dev/null"  /* Path used on ftok for semget key  */
#define SEMKEYID 1              /* Id used on ftok for semget key    */
#define SHMKEYPATH "/dev/null"  /* Path used on ftok for shmget key  */
#define SHMKEYID 1              /* Id used on ftok for shmget key    */

#define NUMSEMS 2
#define SIZEOFSHMSEG 50

int main(int argc, char *argv[])
{
    struct sembuf operations[2];
    void         *shm_address;
    int semid, shmid, rc;
    key_t semkey, shmkey;

semkey = ftok(SEMKEYPATH,SEMKEYID);
shmkey = ftok(SHMKEYPATH,SHMKEYID);
semid = semget( semkey, NUMSEMS, 0666);
shmid = shmget(shmkey, SIZEOFSHMSEG, 0666);
shm_address = shmat(shmid, NULL, 0);

operations[0].sem_num = 0;/* Operate on the first sem      */
operations[0].sem_op =  0;/* Wait for the value to be=0    */
operations[0].sem_flg = 0;/* Allow a wait to occur         */
operations[1].sem_num = 0;/* Operate on the first sem      */
operations[1].sem_op =  1;/* Increment the semval by one   */
operations[1].sem_flg = 0;/* Allow a wait to occur         */

rc = semop( semid, operations, 2 );

	/*working on shared memory*/
		strcpy((char *) shm_address, "hello");
	/*end of writing*/

operations[0].sem_num = 0;/* Operate on the first sem      */
operations[0].sem_op =  -1;/* Decrement the semval by one   */
operations[0].sem_flg = 0;/* Allow a wait to occur         */

operations[1].sem_num = 1;/* Operate on the second sem     */
operations[1].sem_op =  1;/* Increment the semval by one   */
operations[1].sem_flg = 0;/* Allow a wait to occur         */
                                    

rc = semop( semid, operations, 2 );
rc = shmdt(shm_address);
return 0;
}

