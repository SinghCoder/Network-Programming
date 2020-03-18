#include "shell.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>        // for strsep
#include <signal.h>

void terminate(int signo)
{
    // for(int i = 0; i < num_cmds; i++){
    //     char *tmp;
    //     int arg_no = 0;
    //     while((tmp = commands[i][arg_no] )!= NULL)
    //     {
    //         printf("freeing %s\n", tmp);
    //         free(tmp);
    //         arg_no++;
    //     }
    //     printf("freeing commands[%d]\n", i);
    //     free(commands[i]);
    // }
    // printf("freeing commands\n");
    // free(commands);
    // free(input);
}
int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    input = NULL;
    shm_num = 0;
    tcp_sock_num = 0;
    size_t size = 0;
    pid_t pid;
    // setvbuf(stdout, NULL, 0, 0);
    cmd_pipeline pipeline;
    pipeline.first = NULL;
    pipeline.last = NULL;
    pipeline.num_of_cmds = 0;
    for(;;)
    {
        printf("SinghCoder> ");
        getline(&input, &size, stdin);
        if(!input || !strcmp(input, "\n")) continue;
        create_pipeline(input, &pipeline);
        // print_commands(&pipeline);
        execute_commands(&pipeline);
        remove_commands(&pipeline);
        // printf("\n");
        // fflush(stdout);
    }

    signal(SIGINT, &terminate);
    return 0;
}