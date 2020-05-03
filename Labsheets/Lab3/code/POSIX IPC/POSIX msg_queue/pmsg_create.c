#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>


#define QUEUE_NAME  "/test_queue" //Name must begin with /
#define MAX_SIZE    1024



int main(int argc,char *argv[]){
	mqd_t mqd;
    struct mq_attr attr;
    int must_stop = 0;

    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    /* create the message queue */
    mqd = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
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
	
	return 0;
}
