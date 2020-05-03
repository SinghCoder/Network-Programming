#include <signal.h>
#include <stdio.h>
void int_handler(int signo);
main ()
{
  signal (SIGINT, int_handler);
  printf ("Entering infinite loop\n");
  while (1)
    {
      sleep (10);
    }
  printf (" This is unreachable\n");
}

 /* will be called asynchronously, even during a sleep */
void int_handler(int signo)
{
  printf ("Running int_handler\n");
}
