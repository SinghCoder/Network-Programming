#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pipes_n_signals.h"
// #include <sys/stat.h>

static void handler(int sig)
{
    if(sig == SIGUSR2)
    {
        
        printf("Number of messages exchanged between C1 and C2: %d\n", msg_count);
        fflush(NULL);
        kill(0, SIGTERM);
        // exit(1);
    }
    else if(sig == SIGUSR1)
    {
        msg_count++;
        // printf("msg_count %d\n", msg_count);
    }
}

int main(int argc, char *argv[])
{
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);

    int pipe1_2[2];     // send data from c1 to c2
    int pipe2_1[2];     // send data from c2 to c1

    pipe(pipe1_2);
    pipe(pipe2_1);

    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("sigHandler not applied correctly\n");
        exit(1);
    }

    if (sigaction(SIGUSR2, &sa, NULL) == -1)
    {
        perror("sigHandler not applied correctly\n");
        exit(1);
    }   

    pid_t pid_c1 = fork();
    pid_t pid_c2 = -2;

    if(pid_c1 == -1)
    {
        perror("child 1 fork error");
        exit(1);
    }
    if(pid_c1 != 0)
    {   
        pid_c2 = fork();
    }

    if(pid_c2 == -1)
    {
        perror("child 2 fork error\n");
        exit(1);
    }

    if( (pid_c2 > 0) && (pid_c1 > 0) )    //parent
    {
        //close both pipes read write ends in parent
        close( pipe1_2[0] );
        close( pipe1_2[1] );
        close( pipe2_1[0] );
        close( pipe2_1[1] );
    }

    if( (pid_c1 == 0) && (pid_c2 != 0) )//child c1
    {
        close( pipe1_2[0] );    //close read of pipe1_2
        close( pipe2_1[1] );    //close write of pipe2_1
        while (1)
        {
            write( pipe1_2[1], &m , sizeof(m) );
            
            kill(getppid(), SIGUSR1);

            read( pipe2_1[0], &m, sizeof(m) );

            if( m == 0)
            {
                kill(getppid(), SIGUSR2);
                exit(0);
            }
        }
    }

    if(pid_c1 != 0 && pid_c2 == 0)  // child c2
    {
        int t = 0;
        close( pipe1_2[1] ); //close write of pipe1_2
        close( pipe2_1[0] ); //close read of pipe2_1

        while (1)
        {

            read( pipe1_2[0], &t, sizeof(t) );
            kill(getppid(), SIGUSR1);

            t /= n;

            write( pipe2_1[1], &t, sizeof(t) );

            if( t == 0)
            {
                kill(getppid(), SIGUSR2);
                exit(0);
            }
        }
    }

    int status;
    int wpid;

    while ((wpid = wait(&status)) > 0);
    // sleep(10);
    return 0;
}