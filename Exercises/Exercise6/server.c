#define _GNU_SOURCE
#include "helper.h"
#include "server.h"

int listenfd;
int connfd;
char buffer[BUFFERSIZE];
hashTable clientsTable;
clientsInfo *clientsList;

clientNode *makeClient(int fd){
    clientNode *client = (clientNode *)malloc(sizeof(clientNode));
    client->fd = fd;
    client->nextClient = NULL;
    client->prevClient = NULL;
    return client;
}

clientsInfo *initClients(){
    clientsInfo *clients = (clientsInfo *)malloc(sizeof(clientsInfo));

    clients->firstClient = NULL;
    clients->lastClient = NULL;
    clients->numClients = 0;

    return clients;
}

clientsInfo *insertClient(clientsInfo *clients, int fd, hashTable table){

    if(clients == NULL)
        clients = initClients();
    
    if(clients->firstClient == NULL){
        clients->firstClient = clients->lastClient = makeClient(fd);
        clients->numClients = 1;
        insertInTable(table, fd, clients->firstClient);
        return clients;
    }

    clientNode *client = makeClient(fd);
    clients->lastClient->nextClient = client;
    clients->lastClient = client;
    client->prevClient = clients->lastClient;
    clients->numClients++;

    insertInTable(table, fd, client);

    return clients;
}

clientsInfo *removeClient(clientsInfo *clients, int fd, hashTable table){
    clientNode *clientToBeDeleted = searchTable(table, fd);
    
    if(clientToBeDeleted == NULL)
        return clients;
    
    if(clientToBeDeleted != NULL && clientToBeDeleted->prevClient != NULL)
        clientToBeDeleted->prevClient->nextClient = clientToBeDeleted->nextClient;

    if(clientToBeDeleted != NULL && clientToBeDeleted->nextClient != NULL)
        clientToBeDeleted->nextClient->prevClient = clientToBeDeleted->prevClient;
    
    if(clientToBeDeleted == clients->firstClient)
        clients->firstClient = clientToBeDeleted->nextClient;
    
    if(clientToBeDeleted == clients->lastClient)
        clients->lastClient = clientToBeDeleted->prevClient;

    clients->numClients--;

    removeFromTable(table, fd);
    free(clientToBeDeleted);

    return clients;
}

void printClients(clientsInfo *clients){
    clientNode *temp = clients->firstClient;

    printf("Printing clients list\n");
    while(temp){
        printf("Client: %d\n", temp->fd);
        temp = temp->nextClient;
    }
}

void sendToAllExceptSender(clientsInfo *clients, int fd, char *msg){

    clientNode *temp = clients->firstClient;
    
    while(temp){
        
        if(temp->fd != fd){
            Write(temp->fd, msg, strlen(msg));
        }

        temp = temp->nextClient;
    }

}

void sigioListenfdHandler(int sig, siginfo_t *siginfo, void *context){
    printf("connectin? no : %d, for fd : %d, event_band : %ld\n", siginfo->si_signo, (int)siginfo->si_fd, (long)siginfo->si_band);
    fflush(stdout);

    if(siginfo->si_code == POLL_IN) {
        connfd = Accept(listenfd, NULL, NULL);
        fcntl(connfd, F_SETOWN, getpid());
        fcntl(connfd, F_SETFL, fcntl(connfd, F_GETFL) | O_ASYNC | O_NONBLOCK);
        fcntl(connfd, F_SETSIG, SIGRTMIN + 2);

        clientsList = insertClient(clientsList, connfd, clientsTable); 
        printClients(clientsList);
    }

    if(sig == SIGIO){
        printf("Real time signal queue overflow\n");
    }
}

void sigioConnfdHandler(int sig, siginfo_t *siginfo, void *context){
    
    if(siginfo->si_code == POLL_IN) {
        int numRead = Read(siginfo->si_fd, buffer, BUFFERSIZE);
        
        if(numRead == 0){
            Close(siginfo->si_fd);
            clientsList = removeClient(clientsList, siginfo->si_fd, clientsTable);
            printf("socket %d closed\n", siginfo->si_fd);
            printClients(clientsList);
        }
        else if(numRead > 0){
            
            printf("data? no : %d, for fd : %d, event_band : %ld\n", siginfo->si_signo, (int)siginfo->si_fd, (long)siginfo->si_band);
            fflush(stdout);
            buffer[numRead] = '\0';
            printf("Data from connfd %d is [%d bytes] :%s", connfd, numRead, buffer);
            
            sendToAllExceptSender(clientsList, siginfo->si_fd, buffer);
        }
        else{
            // printf("Connection was reset\n");
        }
    }

    if(sig == SIGIO){
        printf("Real time signal queue overflow\n");
    }
}

void usage(char *progName){
    fprintf(stderr, "Usage: %s serverPort\n", progName);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){

    if(argc < 2){
        usage(argv[0]);
    }

    int portNo = atoi(argv[1]);

    struct sigaction saListenfd, saConnfd;
    memset(&saListenfd, 0, sizeof(saListenfd));
    memset(&saConnfd, 0, sizeof(saConnfd));

    sigemptyset(&saListenfd.sa_mask);
    sigemptyset(&saConnfd.sa_mask);

    /* To establish handler for Realtime signals, these flags need to be set*/
    saListenfd.sa_flags = SA_SIGINFO;
    saConnfd.sa_flags = SA_SIGINFO;

    /* Set the handlers */
    saListenfd.sa_sigaction = &sigioListenfdHandler;
    saConnfd.sa_sigaction = &sigioConnfdHandler;

    sigaction(SIGRTMIN + 1, &saListenfd, NULL);
    
    sigaction(SIGRTMIN + 2, &saConnfd, NULL);

    struct sockaddr_in serverAddr, clientAddr;

    memset(&serverAddr, 0, sizeof(serverAddr));
    memset(&clientAddr, 0, sizeof(clientAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portNo);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    if( setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &serverAddr, sizeof(serverAddr)) < 0)
        errorExit("setsockopt failed");
    
    /* Bind the socket and listen for connections */
    Bind(listenfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    Listen(listenfd, BACKLOG);

    /* Set the owner of socket */
    fcntl(listenfd, F_SETOWN, getpid());

    /*Make listening socket nonblocking and asynchronous, make it rcv signal on I/O*/
    int currFlags = fcntl(listenfd, F_GETFL);
    fcntl(listenfd, F_SETFL, currFlags | O_NONBLOCK | O_ASYNC);

    /*Replace SIGIO with RT signal*/
    fcntl(listenfd, F_SETSIG, SIGRTMIN + 1);

    while(true);
}