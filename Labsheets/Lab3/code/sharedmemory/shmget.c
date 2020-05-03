#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>

main ()
{
  int id;
  key_t key;
  int shmid;
  char *data;

  key = ftok ("shmget.c", 'R');

  if ((shmid = shmget (key, 1024, 0644 | IPC_CREAT)) == -1)
    {
      perror ("shmget: shmget failed");
      exit (1);
    }
  data = shmat (shmid, (void *) 0, 0);

  if (data == (char *) (-1))
    perror ("shmat");

/* (b)
 * gets(data); printf("Shared Memory contents : %s\n", data); */
/* (c) if (shmdt(data) == -1) { 	perror("shmdt"); 	exit(1); } if ( shmctl(shmid, IPC_RMID, NULL) == -1 ) { perror("shmctl"); exit(1); } */
  return;
}
