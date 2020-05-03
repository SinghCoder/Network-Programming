#include <unistd.h>
#include <stdio.h>

main ()
{
  printf ("I am running in a process whose deatils are as follows\n");
  printf("process id (pid) = %d, parent process id(ppid) = %d, user id(uid) = %d\n",getpid (), getppid (), getuid ());
}
