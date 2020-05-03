#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define KEY (1492)



union semun
{
  int val;			
  struct semid_ds *buf;		/* Buffer for IPC_STAT, IPC_SET */
  unsigned short *array;	
  struct seminfo *__buf;	
};


void
main ()
{
  int id;	
  struct sembuf operations[2];

  int retval;
  union semun getval;
  unsigned short val[1];
  id = semget (KEY, 1, IPC_CREAT|0666);
  if (id < 0)
    {
      fprintf (stderr, "Producer cannot find semaphore, exiting.\n");
      exit (0);
    }

  /* Set up the sembuf structure. */
  operations[0].sem_num = 0;
  operations[0].sem_op = 0;
  operations[0].sem_flg = 0;



  /* Set up the sembuf structure. */
  operations[1].sem_num = 0;
  /* add 10 to sem value */
  operations[1].sem_op = 10;
  operations[1].sem_flg = 0;

  for (;;)
    {
      retval = semop (id, operations, 2);
      if (retval == 0)
	{
	  printf ("Producer: Adding 10  objects\n");
	  getval.array = val;
	  semctl (id, 0, GETALL, getval);
	  printf ("Sem Val: %d\n", getval.array[0]);
	}
      else
	{
	  printf ("semop: opeartion did not succeed.\n");
	  perror ("REASON");
	}

      sleep (20);
    }
}

