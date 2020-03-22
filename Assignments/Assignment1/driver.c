#include "shell.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>        // for strsep
#include <signal.h>

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    input = NULL;       // initialize input from shell to be NULL
    shm_num = 0;        
    tcp_sock_num = 0;
    size_t size = 0;
    cmd_pipeline pipeline; 
    pipeline.first = NULL;
    pipeline.last = NULL;
    pipeline.num_of_cmds = 0;
    
    for(;;)
    {
        printf("NetP_Shell> ");
    
        getline(&input, &size, stdin);                  //take input
    
        if(!input || !strcmp(input, "\n")) continue;    // if no command is typed simply display console again
    
        create_pipeline(input, &pipeline);              
        execute_commands(&pipeline);                    
        remove_commands(&pipeline);                     // cleanup
    }
    return 0;
}