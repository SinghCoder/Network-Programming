#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int glob = 6;                   //global variable
int
main ()
{
  int var;
  pid_t pid;

  var = 88;
  printf ("Before fork\n");

  if ((pid = fork ()) < 0)
    perror ("fork");   //function to print error that occurred in the process
  else if (pid == 0)
    {
      glob++;
      var++;
      printf ("pid = %d, glob=%d, var=%d\n", getpid (), glob, var);
      exit (0);
                      }
  else
    {
      printf ("pid = %d, glob=%d, var=%d\n", getpid (), glob, var);
      exit (0);
    }
}
