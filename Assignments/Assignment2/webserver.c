#include "webserver.h"

pthread_mutex_t msgqLock = PTHREAD_MUTEX_INITIALIZER;

#define respMsgHeaderTemp "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n"

void *processClients( void *input){
    // printf("am i executing?\n");
    int msqid = *(int *)input;
    msgqbuf msg;

    while(true){
        
        // printf("Will recive a msg\n");
        Pthread_mutex_lock(&msgqLock);
            Msgrcv(msqid, &msg, sizeof(int), /* msgtyp = */ 0, /* msgflags = */ 0);
        Pthread_mutex_unlock(&msgqLock);

        // printf("Received a message from socket %d\n", msg.sockfd);

        clientInfo *clientPtr = searchTable(clientInfoTable, msg.sockfd);

        if(clientPtr == NULL){           
            // printf("lol!! it was NULL\n"); 
            continue;
        }

        state clientState = clientPtr->currState;

        switch(clientState){
            
            case READING_REQUEST:
            {
                // printf("reading request from socket %d\n", msg.sockfd);
                int numRead = Read(msg.sockfd, clientPtr->fr, BUFFERSIZE);

                if(numRead == 0){
                    close(msg.sockfd);
                    continue;
                }
                
                clientPtr->fr[numRead] = '\0';
                printf("Received request: %s\n", clientPtr->fr);
                
                clientPtr->currState = HEADER_PARSING;
                // insertInTable(clientInfoTable, msg.sockfd, clientPtr);
            }
            break;

            case HEADER_PARSING:
            {
                printf("Parsing header\n");

                char firstLine[LINE_SIZE];
                
                sscanf(clientPtr->fr, "%[^\n]", firstLine);


                // printf("Firstline is : %s\n", firstLine);

                char filepath[FILENAME_LEN];
                
                filepath[0] = '.';  // to make it relative to current directory

                sscanf(firstLine + /* skip 'GET '*/4, "%s", filepath + 1 /*1 is to skip the .*/);
                printf("Filepath is %s\n", filepath);
                
                strcpy(clientPtr->filepath, filepath);

                clientPtr->currState = READING_DISKFILE;
                // insertInTable(clientInfoTable, msg.sockfd, clientPtr);
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
                printf("numRead = %d\n", numRead);
                clientPtr->numRead = numRead;

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
                // insertInTable(clientInfoTable, msg.sockfd, clientPtr);
            }
            break;

            case WRITING_HEADER:
            {
                printf("Writing header\n");                
                char header[BUFFERSIZE];
                char length[LINE_SIZE];

                snprintf(header, BUFFERSIZE, respMsgHeaderTemp, getMimeType(clientPtr->filepath + 2 /*2 is to skip ./ */), clientPtr->payloadSize);
                printf("header is %s\n", header);
                Write(msg.sockfd, header, strlen(header));

                clientPtr->currState = WRITING_BODY;
                clientPtr->writtenHeader = true;
                // insertInTable(clientInfoTable, msg.sockfd, clientPtr);
            }
            break;

            case WRITING_BODY:
            {   
                printf("Writing body\n");
                printf("Body is : %s\n", clientPtr->to);
                
                int numWritten = Write(msg.sockfd, clientPtr->to, clientPtr->numRead);
                
                printf("numWritten = %d\n", numWritten);

                if(clientPtr->readCompletely)
                    clientPtr->currState = DONE;
                else
                    clientPtr->currState = READING_DISKFILE;
                // insertInTable(clientInfoTable, msg.sockfd, clientPtr);
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

        // printf("Thread will send msg for next state of client\n");
        // Pthread_mutex_lock(&msgqLock);
        // printf("inserting msg into socket for sockfd %d\n", msg.sockfd);
            Msgsnd(msqid, &msg, sizeof(int), /* msgflags = */ 0);
        // Pthread_mutex_unlock(&msgqLock);
        // printf("And sent\n");
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

    // printf("listenfd : %d\n", listenfd);

    while(true){
        
        // printf("Waiting for epoll wait\n");
        numReady = Epoll_wait(epfd, readyList,/* maxevents = */ NUMTHREADS,/* timeout = */ -1);
        // printf("epoll wait returned %d\n", numReady);
        
        for(int i = 0; i < numReady; i++){

            if( (readyList[i].data.fd == listenfd) && (readyList[i].events & EPOLLIN) ){
                connfd = Accept(listenfd, (struct sockaddr *) &clientAddr, &cliLen);
                // printf("Got connfd as %d\n", connfd);
                struct epoll_event event;
                event.data.fd = connfd;
                event.events = EPOLLIN | EPOLLET;

                // printf("Adding to epoll\n");
                Epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &event);
                
            }
            else if(readyList[i].events & EPOLLIN) {
                // printf("readyList[i] = %d\n", readyList[i].data.fd);
                                
                clientInfo *clientPtr;
                clientPtr = (clientInfo *) malloc(sizeof(clientInfo));
                clientPtr->currState = READING_REQUEST;
                clientPtr->friptr = NULL;
                clientPtr->froptr = NULL;
                clientPtr->toiptr = NULL;
                clientPtr->tooptr = NULL;
                clientPtr->fd = -2;
                clientPtr->writtenHeader = false;
                clientPtr->fileExists = false;
                clientPtr->fileSent = false;
                clientPtr->payloadSize = 0;
                clientPtr->readCompletely = false;

                strcpy(clientPtr->filepath, "");

                insertInTable(clientInfoTable, readyList[i].data.fd, clientPtr);

                msg.mtype = 100;
                msg.sockfd = readyList[i].data.fd;

                // printf("Adding to msg queue\n");
                // printf("inserting msg into socket for sockfd %d\n", msg.sockfd);
                Msgsnd(msgqid, &msg, sizeof(int), /* flag = */ 0);
                // printf("Added to msg queue\n");
            }
        }
    }
}