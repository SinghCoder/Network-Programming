#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define sem_name1 "/Sem1"
#define sem_name2 "/Sem2"
 

int main(int argc,char *argv[]){
		
	int flags,opt;
	mode_t perms;
	unsigned int value;
	sem_t *sem1,*sem2;
	flags = 0;
	flags = flags | O_CREAT | O_RDONLY;
	perms = S_IRUSR;
	value = 1;
	sem1 = sem_open(sem_name1,flags,0777,value);
	value = 0;
	sem2 = sem_open(sem_name2,flags,0777,value);
	if(sem1 < SEM_FAILED){
		perror("Error Creating Semaphore\n");
	}else{
		printf("Semaphore Created Successfully\n");
	}
	if(sem2 < SEM_FAILED){
		perror("Error Creating Semaphore\n");
	}else{
		printf("Semaphore Created Successfully\n");
	}
	pid_t pid = getpid();
	pid_t child = fork();
	if(child==0){
		pid_t pid = getpid();
		for(int  i = 0; i < 100;i++){
			sem_wait(sem2);
			printf("Child Process %d\n",pid);
			sem_post(sem1);
		}
	}else{
		for(int  i = 0; i < 100;i++){
			sem_wait(sem1);
			printf("Parent Process %d\n",pid);
			sem_post(sem2);
		}
		wait(NULL);	
	}
	sem_unlink(sem_name1);
	sem_unlink(sem_name2);
	return 0;
}
