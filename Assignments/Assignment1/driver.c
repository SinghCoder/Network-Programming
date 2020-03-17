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
    input = NULL;
    size_t size = 0;
    pid_t pid;
    // setvbuf(stdout, NULL, 0, 0);
    for(;;)
    {
        printf("$> ");
        getline(&input, &size, stdin);
        parse_input(input);
        // print_commands();
        fflush(stdout);
    }

    signal(SIGINT, &terminate);
    return 0;
}