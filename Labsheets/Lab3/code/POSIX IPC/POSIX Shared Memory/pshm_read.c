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

int main(int argc,char *argv[]){
	int flags,opt,fd;
	mode_t perms;
	size_t size;
	char *addr;
	
	struct stat sb;
	
	flags = O_RDWR | O_CREAT;
	size = 50;
	perms = perms | S_IRUSR | S_IWUSR;
	fd = shm_open(shm_name,flags,perms);
	if(fd==-1){
		perror("Error In Opening\n");
	}
	
	if(fstat(fd,&sb) == -1){
		perror("fstat error\n");
	}
	
	addr = mmap(NULL,size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);;
	if(addr == MAP_FAILED){
		perror("MMAP ERROR\n");
	}
	
	if(close(fd)==-1){
		perror("Close \n"); /* 'fd' is no longer needed */
	}
	
	printf("Reading From Shared Memory\n");
	write(1,addr,sb.st_size);
	printf("\n");
	perror("Read Complete\n");
	return 0;
}
