#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define key (2000)

main ()
{
  int id;
  id = semget (key, 1, IPC_CREAT | 0666);
  if (id < 0)
    printf ("Semaphore is not created\n");
  printf("Semaphore successfully created with id %d\n", id);  
return;
}
