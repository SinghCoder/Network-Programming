#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define key (2000)

main ()
{
  int semid, cnt, ret;
  semid = semget (key, 1, IPC_CREAT | 0666);

  if (semid >= 0)
    {

      /* Read the current semaphore count */
      cnt = semctl (semid, 0, GETVAL);
      if (cnt != -1)
	{
	  printf ("sem count read: current semaphore count %d.\n", cnt);
	}
      else
	perror ("semctl getval:");
/*set sem value*/
      ret = semctl (semid, 0, SETVAL, 6);
      if (ret != -1)
	{
	  perror ("semctl setting sem value");

	}
    }
  else
    perror ("semget:");

  return;
}
