#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <sys/stat.h>
#define MAX_BUF_SIZE 20

int msg_count = 0;

static void sigHandler(int sig)
{
    if(sig == SIGUSR2)
    {
        printf("Number of messages exchanged between C1 and C2: %d", msg_count);
        kill(0, SIGINT);
        exit(1);
    }
    else if(sig == SIGUSR1)
    {
        msg_count++;
    }
}

int main(int argc, char *argv[])
{
    // int m = atoi(argv[1]);
    // int n = atoi(argv[2]);
    char buf_m[MAX_BUF_SIZE];
    char buf_m_by_n[MAX_BUF_SIZE];
    int n = atoi(argv[2]);
    buf_m[MAX_BUF_SIZE - 1] = '\0';
    buf_m_by_n[MAX_BUF_SIZE - 1] = '\0';

    strcpy(buf_m, argv[1]);
    strcpy(buf_m_by_n, argv[2]);

    int pipe1_2[2];     // send data from c1 to c2
    int pipe2_1[2];     // send data from c2 to c1

    pipe(pipe1_2);
    pipe(pipe2_1);

    pid_t pid_c1 = fork();
    pid_t pid_c2 = -2;

    if(pid_c1 == -1)
    {
        perror("child 1 fork error");
        exit(1);
    }
    if(pid_c1 != 0)
    {
        printf("Process with pid %d entering\n", getpid());
        
        pid_c2 = fork();

        if(pid_c2 != 0){
            printf("Parent's pid is %d\n", getpid());
            printf("First Child's pid is %d\n", pid_c1);
            printf("Second Child's pid is %d\n", pid_c2);
        }
    }

    if(pid_c2 == -1)
    {
        perror("child 2 fork error");
        exit(1);
    }

    if( (pid_c2 > 0) && (pid_c1 > 0) )    //parent
    {
        //close both pipes read write ends in parent
        close( pipe1_2[0] );
        close( pipe1_2[1] );
        close( pipe2_1[0] );
        close( pipe2_1[1] );


        if( (signal(SIGUSR1, sigHandler) == SIG_ERR) || (signal(SIGUSR2, sigHandler) == SIG_ERR) )
        {
            perror("sigHandler not applied correctly");
            exit(1);
        }
    }

    if( (pid_c1 == 0) && (pid_c2 != 0) )//child c1
    {
        printf("I have pid %d\n", getpid());
    // }
        close( pipe1_2[0] );    //close read of pipe1_2
        close( pipe2_1[1] );    //close write of pipe2_1
        while (1)
        {
            write( pipe1_2[1], buf_m, strlen(buf_m) );
            
            kill(getppid(), SIGUSR1);
            
            read( pipe2_1[0], buf_m, strlen(buf_m) );

            if( atoi(buf_m) == 0)
            {
                kill(getppid(), SIGUSR2);
                exit(0);
            }
        }
    }

    if(pid_c1 != 0 && pid_c2 == 0)  // child c2
    {
        printf("I have pid %d\n", getpid());
    // }
        close( pipe1_2[1] ); //close write of pipe1_2
        close( pipe2_1[0] ); //close read of pipe2_1

        while (1)
        {

            read( pipe1_2[0], buf_m, strlen(buf_m) );

            kill(getppid(), SIGUSR1);
            
            int m_by_n = atoi(buf_m) / n;

            sprintf(buf_m_by_n, "%d", m_by_n);

            write( pipe2_1[1], buf_m_by_n, strlen(buf_m_by_n) );
            
            if( atoi(buf_m) == 0)
            {
                kill(getppid(), SIGUSR2);
                exit(0);
            }
        }
    }

    // while(1);
    // // int status;
    // wait(&status, NULL);
    // wait(&status, NULL);

    return 0;
}