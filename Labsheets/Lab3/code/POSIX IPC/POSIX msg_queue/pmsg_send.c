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
#define MSG_STOP    "exit"


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
    mqd = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0644, &attr);
    if(mqd < 0)
    	perror("Error While Creating Message Queue\n");
	
	
	printf("Send to server (enter \"exit\" to stop it):\n");

    do {
        printf("> ");
        fflush(stdout);

        memset(buffer, 0, MAX_SIZE);
        fgets(buffer, MAX_SIZE, stdin);

        /* send the message */
        if(mq_send(mqd, buffer, MAX_SIZE, 0) < 0){
        	perror("Error While Sending Message\n");
        }

    } while (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)));
	
	
	return 0;
}
