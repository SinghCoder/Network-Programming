#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <signal.h>

#define QUEUE_NAME  "/test_queue" //Name must begin with /
#define MAX_SIZE    1024
#define NOTIFY_SIG SIGUSR1

extern int errno;

static void handler(int sig){
	printf("Notigication Signal Received\n");
}


int main(int argc,char *argv[]){
	mqd_t mqd;
    struct mq_attr attr;
    char buffer[MAX_SIZE + 1];
    int must_stop = 0;

    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    /* create the message queue */
    mqd = mq_open(QUEUE_NAME, O_CREAT | O_RDWR | O_NONBLOCK, 0644, &attr);
    if(mqd < 0)
    	perror("Error While Creating Message Queue\n");
	
	
	struct mq_attr getattr;
	if(mq_getattr(mqd,&getattr)==-1){
		printf("Error While Fetching Atributes of message queue\n");
		exit(-1);
	}else{
		printf("Maximum Number Of Messages On Queue : %ld\n",getattr.mq_maxmsg);
		printf("Maximum Message Size : %ld\n",getattr.mq_msgsize);
		printf("Number of messages currently on Queue : %ld\n",getattr.mq_curmsgs);
	}
	
	/* Receiving Signal */
	
	struct sigevent sev;
	sigset_t blockMask, emptyMask;
	struct sigaction sa;
	sigemptyset(&blockMask);
	sigaddset(&blockMask,NOTIFY_SIG);
	if(sigprocmask(SIG_BLOCK, &blockMask,NULL)==-1){
		perror("Error In Blocking The Signal\n");
	}
	
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	if(sigaction(NOTIFY_SIG,&sa,NULL)==-1){
		perror("Error While Setting Signal Handler\n");
	}
	
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = NOTIFY_SIG;
	if(mq_notify(mqd,&sev)==-1){
		perror("Error In Registeration For Signal notification\n");
	}
	
	sigemptyset(&emptyMask);
	ssize_t numRead;
	for(;;){
		sigsuspend(&emptyMask); /* Wait For Signal Notification */
		
		if(mq_notify(mqd,&sev) == -1){
			perror("Error In Registeration For Signal notification\n");
		}		
		
		while((numRead = mq_receive(mqd,buffer,attr.mq_msgsize,NULL)) >= 0){
			printf("Message Read Is %s",buffer);
			printf("Read %ld bytes \n",(long)numRead);
		}
	}	
	return 0;
}
