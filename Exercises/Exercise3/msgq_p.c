#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>
#include "msg_queue.h"
#include <wait.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    pid_t c0_pid = fork();

    int msgq_id = msgget(MSG_Q_KEY, IPC_CREAT);
    if(msgq_id == -1)
    {
	    perror("Main process | Message queue creation failed\n");
	    exit(0);
    }
    printf("Main process | Msg queue ID : %d\n", msgq_id);

    if(0 == c0_pid) //child
    {
        char *path_child = argv[1];
        if( execl(path_child ,path_child ,(char*)NULL) == -1)
            perror("execl : ");
    }

    char str[MAX_LINE_LENGTH];
    size_t msg_size;
    size_t len_rcvd;
    message m_write;
    message m_read;
    
    while(  fgets(str, MAX_LINE_LENGTH, stdin) != NULL )
    {
        m_write.mtype = MSG_TYPE_P;
        m_read.mtype = MSG_TYPE_C0;
        
        strcpy(m_write.mtext, str);
        msg_size = strlen(m_write.mtext);

        if(msgsnd(msgq_id, &m_write, msg_size, 0) == -1){
            perror("Process P | msgsnd error :  ");
            exit(0);
        }
        
        if( (len_rcvd = msgrcv(msgq_id, &m_read, MAX_MSG_SIZE , MSG_TYPE_C0, 0) ) == -1){
            perror("Process P | msgrcv error :  ");
            exit(0);
        }
        
        m_read.mtext[len_rcvd] ='\0';

        printf("Process P | Message received : %s Mesage type : %ld\n", m_read.mtext, m_read.mtype);
        strcpy(str,"");
    }

    if( msgctl(msgq_id, IPC_RMID, NULL) == -1)
    {
        perror("Process P | Error in closing message queue\n");
    }

    int status;
    wait(&status);
    return 0;
}
