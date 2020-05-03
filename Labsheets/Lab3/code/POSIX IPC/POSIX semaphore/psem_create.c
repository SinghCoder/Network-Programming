#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>

#define sem_name "/tempSem" 

int main(int argc,char *argv[]){
		
	int flags,opt;
	mode_t perms;
	unsigned int value;
	sem_t *sem;
	flags = 0;
	flags = flags | O_CREAT | O_RDONLY;
	perms = S_IRUSR;
	value = 1;
	sem = sem_open(sem_name,flags,0777,value);
	if(sem < SEM_FAILED){
		perror("Error Creating Semaphore\n");
	}else{
		printf("Semaphore Created Successfully\n");
	}
	int currval = 0;
	if(sem_getvalue(sem,&currval) < 0){
		perror("Error While Getting The Value Of Semaphore\n");
	}else{
		printf("Current Value of Semaphore Is :\t%d\n",currval);
	}
	sem_unlink(sem_name);
	
	return 0;
}
