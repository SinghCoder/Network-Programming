#include <stdio.h>      
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <stdlib.h>     
#include <string.h>     
#include <unistd.h>     
#include <signal.h>
#define RCVBUFSIZE 32   

void DieWithError(char *errorMessage);  

void handler(int signo)
{
printf("SIG PIPE received");
}
int main(int argc, char *argv[])
{

    void handler(int);
    signal(SIGPIPE,handler);
    int sock;                        
    struct sockaddr_in echoServAddr; 
    unsigned short echoServPort;     
    char *servIP;                    
    char *echoString;                
    char echoBuffer[RCVBUFSIZE];     
    unsigned int echoStringLen;      
    int bytesRcvd, totalBytesRcvd;   
    
    if ((argc < 3) || (argc > 4))        {
       fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n",
               argv[0]);
       exit(1);
    }

    servIP = argv[1];             
    echoString = argv[2];         

    if (argc == 4)
        echoServPort = atoi(argv[3]); 
    else
        echoServPort = 7;  

    
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    
    memset(&echoServAddr, 0, sizeof(echoServAddr));     

    echoServAddr.sin_family      = AF_INET;                     
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   
    echoServAddr.sin_port        = htons(echoServPort); 

    
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("connect() failed");

    char sendLine[100],recvLine[100];
    
    printf("pid: %d",getpid());
    
    while(fgets(sendLine,sizeof(sendLine),stdin)!=NULL)
    {
    send(sock,sendLine,strlen(sendLine),0);
    sleep(1);
    send(sock,sendLine,strlen(sendLine),0);
        if ((bytesRcvd = recv(sock, recvLine, 99, 0)) <= 0)
        {
        perror("recv error");
        }
    puts(recvLine);
    }
    
    
	return(0);
} 
