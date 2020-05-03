#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>

main ()
{
  int id,i;
  key_t key;
  int pid;
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

  if((pid==fork())==0)
{

for(i=0;i<50;i++)
{	printf("%s", data);
	sprintf(data, "I am child and my pid is %d and my count is %d\n", getpid(), i);
}
exit(0);
}
else if(pid>0)
{
	
for(i=0;i<50;i++)
{	printf("%s", data);
	sprintf(data, "I am parent and my pid is %d and my count is %d\n", getpid(), i);
} 
}
  return;
}
