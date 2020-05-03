#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define shm_name "/temp_shm"
#define message "HELLO World"

int main(int argc,char *argv[]){
	int flags,opt,fd;
	mode_t perms;
	size_t size;
	char *addr;
	
	flags = O_RDWR | O_CREAT;
	size = 50;
	perms = perms | S_IRUSR | S_IWUSR;
	fd = shm_open(shm_name,flags,perms);
	if(fd==-1){
		perror("Error In Opening\n");
	}
	
	size = strlen(message);
	if(ftruncate(fd,size)==-1){	/*Resize Object To Hold String*/
		perror("Truncate error\n");
	}
	printf("Shared Memory Size Resized To %ld Bytes\n",(long)size);
	addr = mmap(NULL,size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);;
	if(addr == MAP_FAILED){
		perror("MMAP ERROR\n");
	}
	
	if(close(fd)==-1){
		perror("Close \n"); /* 'fd' is no longer needed */
	}
	
	printf("Copying %ld bytes\n",(long) size);
	memcpy(addr,message,size);
	perror("Writing Message To Shared Memory\n");
	return 0;
}
