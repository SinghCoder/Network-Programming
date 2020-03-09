#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#define MSG_Q_KEY 1000
#define MSG_TYPE_P 2
#define MSG_TYPE_C0 1
#define MAX_LINE_LENGTH 100
#define MAX_MSG_SIZE 200

typedef enum{
    false,
    true
} bool;

typedef struct msg
{
    long mtype;
    char mtext[MAX_LINE_LENGTH];
} message;

#endif
