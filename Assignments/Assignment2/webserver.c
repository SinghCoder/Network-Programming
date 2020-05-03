#include "webserver.h"

pthread_mutex_t msgqLock = PTHREAD_MUTEX_INITIALIZER;

#define respMsgHeaderTemp "HTTP/1.1 %d %s\r\nDate: %s\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n"

void *processClients( void *input){
    int msqid = *(int *)input;
    msgqbuf msg;

    while(true){
        
        Pthread_mutex_lock(&msgqLock);
            Msgrcv(msqid, &msg, sizeof(int), /* msgtyp = */ 0, /* msgflags = */ 0);
        Pthread_mutex_unlock(&msgqLock);

        clientInfo *clientPtr = searchTable(clientInfoTable, msg.sockfd);

        if(clientPtr == NULL){
            continue;
        }

        state clientState = clientPtr->currState;

        switch(clientState){
            
            case READING_REQUEST:
            {
                printf("reading request\n");
                int numRead = Read(msg.sockfd, clientPtr->friptr, &(clientPtr->fr[BUFFERSIZE]) - clientPtr->friptr);
                printf("req is : %s\n", clientPtr->fr);
                if(numRead == 0){
                    Close(msg.sockfd);
                    continue;
                }
                else if(numRead < 0){
                    if(errno != ECONNRESET && errno != EBADF)
                        Msgsnd(msqid, &msg, sizeof(int), /* msgflags = */ 0);
                    continue;
                }
                else{
                    clientPtr->friptr += numRead;
                    char *endPtr = clientPtr->friptr - 1;

                    *(clientPtr->friptr) = '\0';

                    if( (clientPtr->friptr - clientPtr->fr >= 4) && (*endPtr == '\n') && (*(endPtr - 1) == '\r') && (*(endPtr - 2) == '\n') && (*(endPtr - 3) == '\r') ){                
                        clientPtr->currState = HEADER_PARSING;       
                    }                    
                }
            }
            break;

            case HEADER_PARSING:
            {
                printf("Parsing header\n");

                char firstLine[LINE_SIZE];
                
                sscanf(clientPtr->fr, "%[^\n]", firstLine);

                char filepath[FILENAME_LEN];
                
                filepath[0] = '.';  // to make it relative to current directory

                sscanf(firstLine + /* skip 'GET '*/4, "%s", filepath + 1 /*1 is to skip the .*/);
                
                strncpy(clientPtr->filepath, filepath, FILENAME_LEN);

                clientPtr->currState = READING_DISKFILE;
            }
            break;

            case READING_DISKFILE:
            {
                if(clientPtr->fd == -2)
                    clientPtr->fd = Open(clientPtr->filepath, O_RDONLY);

                if(clientPtr->fd < 0){
                    
                    clientPtr->fileExists = false;
                    strcpy(clientPtr->filepath, "./fileDne.html");
                    
                    clientPtr->fd = Open(clientPtr->filepath, O_RDONLY);
                }
                else{                    
                    clientPtr->fileExists = true;                    
                }
                printf("Reading diskfile %s\n", clientPtr->filepath);

                int numRead = Read(clientPtr->fd, clientPtr->to, BUFFERSIZE);
                clientPtr->numRead = numRead;
                
                clientPtr->toiptr += numRead;

                if(clientPtr->writtenHeader == false){
                    int currOffset = Lseek(clientPtr->fd, 0, SEEK_CUR);
                    
                    clientPtr->payloadSize = Lseek(clientPtr->fd, 0, SEEK_END);
                    Lseek(clientPtr->fd, currOffset, SEEK_SET);

                    if(currOffset == clientPtr->payloadSize){
                        clientPtr->readCompletely = true;                        
                    }

                    clientPtr->currState = WRITING_HEADER;
                }
                else{
                    int currOffset = Lseek(clientPtr->fd, 0, SEEK_CUR);

                    if(currOffset == clientPtr->payloadSize){
                        clientPtr->readCompletely = true;
                    }

                    clientPtr->currState = WRITING_BODY;
                }

                if(clientPtr->readCompletely)
                    Close(clientPtr->fd);
            }
            break;

            case WRITING_HEADER:
            {
                printf("Writing header\n");

                if(clientPtr->fileExists)
                    snprintf(clientPtr->hdriptr, BUFFERSIZE, respMsgHeaderTemp, 200, "OK", getTimestamp(),
                                getMimeType(clientPtr->filepath + 2 /*2 is to skip ./ */),clientPtr->payloadSize);
                else
                    snprintf(clientPtr->hdriptr, BUFFERSIZE, respMsgHeaderTemp, 404, "Not Found", getTimestamp(),
                                getMimeType(clientPtr->filepath + 2 /*2 is to skip ./ */),clientPtr->payloadSize);

                clientPtr->hdriptr += strlen(clientPtr->hdriptr);

                int numWritten = Write(msg.sockfd, clientPtr->hdroptr, clientPtr->hdriptr - clientPtr->hdroptr);

                if(numWritten < 0) {
                    if(errno != ECONNRESET && errno != EBADF)
                        Msgsnd(msqid, &msg, sizeof(int), /* msgflags = */ 0);
                    continue;
                }
                else if(numWritten == 0) {
                    Close(msg.sockfd);
                    continue;
                }
                else{
                    clientPtr->hdroptr += numWritten;
                    if(clientPtr->hdroptr == clientPtr->hdriptr){
                        clientPtr->hdriptr = clientPtr->hdroptr = clientPtr->header;
                        clientPtr->currState = WRITING_BODY;
                        clientPtr->writtenHeader = true;
                    }
                }
            }
            break;

            case WRITING_BODY:
            {   
                printf("Writing body\n");
                
                int numWritten = Write(msg.sockfd, clientPtr->tooptr, clientPtr->toiptr - clientPtr->tooptr);
                
                if(numWritten < 0){
                    if(errno != ECONNRESET && errno != EBADF)
                        Msgsnd(msqid, &msg, sizeof(int), /* msgflags = */ 0);
                    continue;
                }
                else if(numWritten == 0) {
                    Close(msg.sockfd);
                    continue;
                }
                else{
                    clientPtr->tooptr += numWritten;
                    if(clientPtr->tooptr == clientPtr->toiptr){
                        clientPtr->toiptr = clientPtr->tooptr = clientPtr->to;
                        if(clientPtr->readCompletely)
                            clientPtr->currState = DONE;
                        else
                            clientPtr->currState = READING_DISKFILE;
                    }
                }
            }
            break;

            case DONE:
            {
                strcpy(clientPtr->fr, "");
                strcpy(clientPtr->to, "");
                printf("in done state\n");
                removeFromTable(clientInfoTable, msg.sockfd);
                continue;
            }
            break;

            default:
            {
                printf("It should not have come here!! Something's wrong..\n");
                exit(EXIT_FAILURE);
            }
        }
        
        Msgsnd(msqid, &msg, sizeof(int), /* msgflags = */ 0);
    }

    return NULL;
}

int main(){

    initTable(clientInfoTable);
    pthread_t thread[NUMTHREADS];

    int msgqid = Msgget(IPC_PRIVATE, 0666);

    for(int i = 0; i < NUMTHREADS; i++){
        Pthread_create(&thread[i], NULL, processClients, (void *) &msgqid);
    }

    int epfd = Epoll_create1(0); 

    int listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr, clientAddr;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);

    // for allowing binding again
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
        errorExit("setsockopt(SO_REUSEADDR) failed");

    Bind(listenfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    Listen(listenfd, MAX_QUEUE_SIZE);   

    struct epoll_event ev;
    ev.data.fd = listenfd;
    ev.events = EPOLLIN | EPOLLET;

    Epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

    struct epoll_event readyList[NUMTHREADS];
    int numReady = 0;
    int connfd = -1;
    struct msgqbuf msg;
    int cliLen = sizeof(clientAddr);

    while(true){
        numReady = Epoll_wait(epfd, readyList,/* maxevents = */ 20,/* timeout = */ -1);
        
        for(int i = 0; i < numReady; i++){

            if( (readyList[i].data.fd == listenfd) && (readyList[i].events & EPOLLIN) ){
                connfd = Accept(listenfd, (struct sockaddr *) &clientAddr, &cliLen);
                struct epoll_event event;
                event.data.fd = connfd;
                event.events = EPOLLIN | EPOLLET;

                int status = fcntl(connfd, F_SETFL, fcntl(connfd, F_GETFL, 0) | O_NONBLOCK);

                if (status == -1){
                    errorExit("fcntl failed");
                }
                Epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &event);
                
            }
            else if(readyList[i].events & EPOLLIN) {
                                
                clientInfo *clientPtr;
                clientPtr = (clientInfo *) malloc(sizeof(clientInfo));
                clientPtr->currState = READING_REQUEST;
                clientPtr->friptr = clientPtr->fr;
                clientPtr->froptr = clientPtr->fr;
                clientPtr->toiptr = clientPtr->to;
                clientPtr->tooptr = clientPtr->to;
                clientPtr->hdriptr = clientPtr->header;
                clientPtr->hdroptr = clientPtr->header;
                clientPtr->fd = -2;
                clientPtr->writtenHeader = false;
                clientPtr->fileExists = false;
                clientPtr->payloadSize = 0;
                clientPtr->readCompletely = false;

                strcpy(clientPtr->filepath, "");
                strcpy(clientPtr->reqLastfourCharsStr, "");

                insertInTable(clientInfoTable, readyList[i].data.fd, clientPtr);

                msg.mtype = 100;
                msg.sockfd = readyList[i].data.fd;

                Msgsnd(msgqid, &msg, sizeof(int), /* flag = */ 0);
            }
        }
    }
}