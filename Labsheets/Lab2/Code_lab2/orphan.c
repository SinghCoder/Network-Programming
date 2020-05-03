# include <stdio.h>

int main()
{    
    int pid;
    pid=getpid();
    
    printf("Current Process ID is : %d\n",pid);

    printf("[ Forking Child Process ... ] \n");    
    pid=fork(); 
if(pid < 0)
    {
        /* Process Creation Failed ... */

        exit(-1);
    }
    else if(pid==0) 
    {
        /* Child Process */

        printf("Child Process is Sleeping ...");
        sleep(5);

        /* 
 *            Orphan Child's Parent ID is 1
 *                            */

        printf("\nOrphan Child's Parent ID : %d",getppid());
    }
    else 
    {
        /* Parent Process */

        printf("Parent Process Completed ...");
    }    
    return 0;
}

