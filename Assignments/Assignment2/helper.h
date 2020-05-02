#ifndef HELPER_H
#define HELPER_H

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for read() / write()
#include <errno.h>
#include <pthread.h>
#define BUFFERSIZE 1000
#define SUCCESS 1

/* Client info */
typedef enum state {READING_REQUEST, HEADER_PARSING, READING_DISKFILE, WRITING_HEADER, WRITING_BODY, DONE} state;
typedef struct clientInfo{
    state currState;
    char fr[BUFFERSIZE];
    char to[BUFFERSIZE];
    char *friptr;
    char *froptr;
    char *toiptr;
    char *tooptr;
} clientInfo;

/* Message queue functions */
typedef enum {false, true} bool;

typedef struct msgqbuf{
    long mtype;
    int sockfd;
} msgqbuf;

int Msgget(key_t key, int msgflg);

int Msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);

ssize_t Msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);

int Msgctl(int msqid, int cmd, struct msqid_ds *buf);


/* Exit on system call error function*/
void error_exit(char *msg);

/* Socket functions */

int Listen(int sockfd, int backlog);

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

ssize_t Read(int fd, void *buf, size_t count);

ssize_t Write(int fd, const void *buf, size_t count);

/* epoll functions */

int Epoll_create(int size);

int Epoll_ctl(int epfd, int op, int fd, struct epoll_event *ev);

int Epoll_wait(int epfd, struct epoll_event *evlist, int maxevents, int timeout);

/* pthread functions */
int Pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

int Pthread_join(pthread_t thread, void **retval);

int Pthread_detach(pthread_t thread);

pthread_t Pthread_self(void);

void Pthread_exit(void *retval);

#endif