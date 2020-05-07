#include "helper.h"

void errorExit(char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
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

ssize_t Read(int fd, void *buf, size_t count){
    int numRead = read(fd, buf, count);
    if( numRead < 0){
        if(errno != ECONNRESET && errno != EBADF)
            errorExit("read failed");
    }
    
    return numRead;
}

ssize_t Write(int fd, const void *buf, size_t count){
    int numWritten = write(fd, buf, count);
    
    if(numWritten < 0){
        if(errno != EWOULDBLOCK && errno != EAGAIN){
            if(errno != ECONNRESET && errno != EBADF)
                errorExit("write failed");
        }
    }

    return numWritten;
}

int Close(int fd){
    if(close(fd) < 0 ){
        if(errno != EBADF)
            errorExit("close failed");
    }

    return SUCCESS;
}

char *getTimestamp(){
    char *buf = (char*)malloc(sizeof(char) * 1000);
    time_t now = time(0);

    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    
    return buf;
}
