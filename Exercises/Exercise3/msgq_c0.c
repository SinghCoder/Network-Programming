#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <ctype.h>
#include <sys/types.h>
#include "msg_queue.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int msgq_id = msgget(MSG_Q_KEY, IPC_CREAT);

    if(msgq_id == -1){
	    perror("msgq creation failed\n");
	    exit(0);
    }

    printf("C0 process | Msg queue ID : %d\n", msgq_id);

    char *str;
    size_t msg_size;
    size_t msg_len;
    message m_write;
    message m_read;
    m_write.mtype = MSG_TYPE_C0;
    m_read.mtype = MSG_TYPE_P;

    while( true )
    {        
        if( (msg_len = msgrcv(msgq_id, &m_read, MAX_MSG_SIZE, MSG_TYPE_P, 0) ) == -1)
        {
            perror("Process C0 | msgrcv error : ");
            exit(0);
        }

        m_read.mtext[msg_len] = '\0';
        printf("Process C0 | Message received : %s Message type : %ld\n",m_read.mtext, m_read.mtype);

        int i; 

        for(i = 0; i < strlen( m_read.mtext ) ; i++)
        {
            m_write.mtext[i] = toupper( m_read.mtext[i] );
        }

        // m_write.mtext[i] = '\0';
        
        msg_size = strlen(m_read.mtext);

        if(msgsnd(msgq_id, &m_write, msg_size, 0) == -1){
            perror("Process C0 : msgsnd error : ");
            exit(0);
        }
    }

    return 0;
}

