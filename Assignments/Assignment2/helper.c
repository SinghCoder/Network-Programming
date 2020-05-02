#include "helper.h"

void errorExit(char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

void errorExitPthread(char *msg, int errnum){
    printf("%s : %s\n",msg, strerror(errnum));
    exit(EXIT_FAILURE);
}

int Msgget(key_t key, int msgflg){
    int msqid = msgget(key, msgflg);
    if( msqid < 0){
        errorExit("msgget failed");
    }

    return msqid;
}

int Msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg){
    if(msgsnd(msqid, msgp, msgsz, msgflg) < 0){
        errorExit("msgsnd failed");
    }

    return SUCCESS;
}

ssize_t Msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg){
    ssize_t numrcvd = msgrcv(msqid, msgp, msgsz, msgtyp, msgflg);
    if( numrcvd < 0){
        errorExit("msgrcv failed");
    }

    return numrcvd;
}

int Msgctl(int msqid, int cmd, struct msqid_ds *buf){
    if(msgctl(msqid, cmd, buf) < 0){
        errorExit("msgcrl failed");
    }

    return SUCCESS;
}


int Listen(int sockfd, int backlog){
    if(listen(sockfd, backlog) < 0){
        errorExit("listen failed");
    }

    return SUCCESS;
}

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen){
    if(bind(sockfd, addr, addrlen) < 0){
        errorExit("bind failed");
    }

    return SUCCESS;
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen){
    if(accept(sockfd, addr, addrlen) < 0){
        errorExit("accept failed");
    }

    return SUCCESS;
}

ssize_t Read(int fd, void *buf, size_t count){
    int numRead = read(fd, buf, count);
    if( numRead < 0){
        if(errno != EWOULDBLOCK && errno != EAGAIN){
            errorExit("read failed");
        }
    }

    return numRead;
}

ssize_t Write(int fd, const void *buf, size_t count){
    int numWritten = write(fd, buf, count);
    
    if(numWritten < 0){
        if(errno != EWOULDBLOCK && errno != EAGAIN){
            errorExit("read failed");
        }
    }

    return numWritten;
}

int Epoll_create(int size){
    int epfd = epoll_create(size);

    if(epfd < 0){
        errorExit("epoll_create failed");
    }

    return epfd;
}

int Epoll_ctl(int epfd, int op, int fd, struct epoll_event *ev){
    if(epoll_ctl(epfd, op, fd, ev) < 0){
        errorExit("epoll_ctl failed");
    }

    return SUCCESS;
}

int Epoll_wait(int epfd, struct epoll_event *evlist, int maxevents, int timeout){
    int num_epfds = epoll_wait(epfd, evlist, maxevents, timeout);

    if(num_epfds < 0){
        errorExit("epoll_wait failed");
    }

    return num_epfds;
}

int Pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg){
    int status = pthread_create(thread, attr, start_routine, arg);
    
    if(status > 0){
        errorExitPthread("pthread_create failed", status);
    }

    return SUCCESS;
}

int Pthread_join(pthread_t thread, void **retval){
    int status = pthread_join(thread, retval);

    if(status > 0){
        errorExitPthread("pthread_create failed", status);
    }

    return SUCCESS;
}

int Pthread_detach(pthread_t thread){

    int status = pthread_detach(thread);

    if(status > 0){
        errorExitPthread("pthread_create failed", status);
    }

    return SUCCESS;
}

pthread_t Pthread_self(void){
    return pthread_self();
}

void Pthread_exit(void *retval){
    pthread_exit(retval);
}
