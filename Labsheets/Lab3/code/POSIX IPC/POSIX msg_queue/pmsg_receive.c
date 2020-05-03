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
	
	
	do {
        ssize_t bytes_read;

        /* receive the message */
        bytes_read = mq_receive(mqd, buffer, MAX_SIZE, NULL);
        if(bytes_read < 0){
        	perror("Error While Receiving Message\n");
        }
        buffer[bytes_read] = '\0';
        if (! strncmp(buffer, MSG_STOP, strlen(MSG_STOP)))
        {
            must_stop = 1;
        }
        else
        {
            printf("Received: %s\n", buffer);
        }
    } while (!must_stop);
	
	
	return 0;
}
