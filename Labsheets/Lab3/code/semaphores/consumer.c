#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define KEY (1492)
union semun
{
  int val;			
  struct semid_ds *buf;	
  unsigned short *array;
  struct seminfo *__buf;				
};

void
main ()
{
  int id;			
  struct sembuf operations[1];

  int retval;

  union semun setval;
  unsigned short val[1];

  id = semget (KEY, 1, IPC_CREAT | 0666);
  if (id < 0)
    {
      fprintf (stderr, "Consumer cannot find semaphore, exiting.\n");
      exit (0);
    }


  operations[0].sem_num = 0;
  /* Subtract 1 from semaphore value : */
  operations[0].sem_op = -1;
  operations[0].sem_flg = 0;

  for (;;)
    {
      retval = semop (id, operations, 1);
      if (retval == 0)
	{
	  printf ("Consumer: Getting one object from shelf.\n");
	      setval.array=val;
  		semctl (id, 0, GETALL, setval);
	printf("Sem Value: %d\n", setval.array[0]);
	}
      else
	{
	  printf ("Consumer: operation did not succeed.\n");
	}

	sleep(2);
    }
}

