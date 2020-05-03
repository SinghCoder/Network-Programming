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

int Socket(int domain, int type, int protocol){

    int sockfd = socket(domain, type, protocol);

    if( sockfd < 0){
        errorExit("socket failed");
    }

    return sockfd;
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
    int connfd = accept(sockfd, addr, addrlen);
    if( connfd < 0){
        errorExit("accept failed");
    }

    return connfd;
}

int Open(const char *pathname, int flags){
    int fd = open(pathname, flags);

    // if(fd < 0){
    //     errorExit("open failed");
    // }

    return fd;
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

off_t Lseek(int fd, off_t offset, int whence){
    off_t offsetVal = lseek(fd, offset, whence);

    if(offsetVal < 0){
        errorExit("lseek failed");
    }

    return offsetVal;
}

int Epoll_create1(int flags){
    int epfd = epoll_create1(flags);

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

int Pthread_mutex_lock(pthread_mutex_t *mutex){

    int status = pthread_mutex_lock(mutex);

    if(status > 0){
        errorExitPthread("pthread_create failed", status);
    }

    return SUCCESS;

}

int Pthread_mutex_unlock(pthread_mutex_t *mutex){

    int status = pthread_mutex_unlock(mutex);

    if(status > 0){
        errorExitPthread("pthread_create failed", status);
    }

    return SUCCESS;

}

char *tolowerStr(char *str){
    int len = strlen(str);

    for(int i = 0; i < len; i++){
        str[i] = tolower(str[i]);
    }

    return str;
}

char *getMimeType(char *filename){
    char *extension = strchr(filename, '.');

    if(extension == NULL){
        return DEFAULT_MIME_TYPE;
    }

    extension = extension + 1; /* skip . */
    extension = tolowerStr(extension);

    printf("extension is %s\n", extension);

    if (strcmp(extension, "html") == 0 || strcmp(extension, "htm") == 0) { 
        return "text/html"; 
    }
    if (strcmp(extension, "jpeg") == 0 || strcmp(extension, "jpg") == 0) { 
        return "image/jpg"; 
    }
    if (strcmp(extension, "css") == 0) { 
        return "text/css"; 
    }
    if (strcmp(extension, "js") == 0) { 
        return "application/javascript"; 
    }
    if (strcmp(extension, "json") == 0) { 
        return "application/json"; 
    }
    if (strcmp(extension, "txt") == 0) { 
        return "text/plain"; 
    }
    if (strcmp(extension, "gif") == 0) { 
        return "image/gif"; 
    }
    if (strcmp(extension, "png") == 0) { 
        return "image/png"; 
    }

    return DEFAULT_MIME_TYPE;
}
