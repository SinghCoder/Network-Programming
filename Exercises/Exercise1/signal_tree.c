#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
int points = 0 ;
int A = 0;
int S = 0;
int child1pid = -1;
int child2pid = -1;
int *validPIDs = NULL ;

static struct sigaction siga;
void sigHandler(int sig, siginfo_t *si, void *context)
{
    int sender_pid = si->si_pid ;

    if(sender_pid == getppid())
    {
        printf(" from parent\n");
        points += A;
    }

    else if(sender_pid == child1pid || sender_pid == child2pid)
    {
        printf(" from child\n");
        points -= S;
    }
    else if( abs(sender_pid - getpid()) == 1)
    {
        printf(" from sibling\n");
        points -= S/2;
    }


    printf("Pid : %d\n",getpid());
    printf ("ppid : %d\n",getppid());
    printf ("Children : %d , %d\n",child1pid, child2pid);

    printf("Signal received from : %d\n",sender_pid);

    printf("Points right now : %d\n\n",points);

    if(points == 0)
    {
        printf("No points remaining\n");
        exit(0);
    }
}

int main(int argc, char* argv[])
{
    int N = atoi(argv[1]);
    int lvl = (N+1)/2;    
    int root = getpid();

    A = atoi(argv[2]);
    S = atoi(argv[3]);

    points = N;
    validPIDs = (int *)malloc(sizeof(int)*N);
    int counter = 0;
    for(int i=0; i < lvl; i++)
    {
        child1pid = 0;
        child2pid = 0;
        
        if((child1pid = fork()) == 0)
        {
            break;
        }
        if(i == lvl -1 && N%2 != 0)
        {
            break;
        }
        if((child2pid = fork()) == 0)
        {
            continue;
        }
        break;
    }
    
    
    
    siga.sa_sigaction = *sigHandler;
    siga.sa_flags |= SA_SIGINFO;

    if( sigaction(SIGCHLD, &siga, NULL) != 0)
    {
        printf(" Error in applying signal handler\n");
        exit(0); 
    }

    int pid = getpid();
    int ppid = getppid();

    for(int id = ppid - N; id <= pid + N; id++)
    {
        kill(id, SIGCHLD);
        sleep(2);
    }

    // sleep(30);
    return 0;
}