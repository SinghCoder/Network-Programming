#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int
main()
{
pid_t pid;

pid=fork();

if(pid<0)
        perror("fork:");
else if(pid==0){
        printf("In Child: pid = %d, parent pid= %d", getpid(), getppid());
        printf("child finishing...");
        exit(0);
}
else if (pid>0){
while(1);
}

}
