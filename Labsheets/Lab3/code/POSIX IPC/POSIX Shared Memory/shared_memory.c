#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>

#define shm_name "/temp_shm"

int main(int argc,char *argv[]){
	int flags,opt,fd;
	mode_t perms;
	size_t size;
	void *addr;
	
	flags = O_RDWR | O_CREAT;
	
	size = 50;
	perms = perms | S_IRUSR | S_IWUSR;
	fd = shm_open(shm_name,flags,perms);
	if(fd==-1){
		perror("Error In Opening\n");
	}
	addr = mmap(NULL,size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);;
	if(addr == MAP_FAILED){
		perror("MMAP ERROR\n");
	}
	return 0;
}
