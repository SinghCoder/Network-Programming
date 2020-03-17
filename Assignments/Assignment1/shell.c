#include "shell.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>        // for strsep
#include <ctype.h>
#include <fcntl.h>
#include <wait.h>

void error_exit(char *err_msg)
{
    perror(err_msg);
    exit(EXIT_FAILURE);
}

void initialize_cmd(struct command **cmd_ptr){
    *cmd_ptr = (struct command*)malloc(sizeof(struct command));
    
    if(*cmd_ptr == NULL)
        error_exit("malloc:");

    (*cmd_ptr)->argc = 0;
    
    strcpy((*cmd_ptr)->input_file, "");
    strcpy((*cmd_ptr)->output_file, "");
    
    (*cmd_ptr)->input_redirect = 0;
    (*cmd_ptr)->output_redirect = 0;
    (*cmd_ptr)->output_append = 0;
    (*cmd_ptr)->next = NULL;
    
    for(int i=0 ; i < NUM_OF_ARGS; i++)
        (*cmd_ptr)->argv[i] = NULL;
}

void insert_command(cmd_pipeline *pipeline, struct command *cmd)
{
    if(pipeline->first == NULL){
        pipeline->first = cmd;
        pipeline->last = cmd;
        return;
    }
    
    pipeline->last->next = cmd;
    pipeline->last = cmd;
}

void parse_command(struct command **cmd_ptr, char *input)
{
    int len = strlen(input);
    char *str = (char*)malloc(sizeof(char)*(len+1));
    
    if(str == NULL)
        error_exit("malloc str");

    char prev_str[ARG_LENGTH];
    int arg_size = 0;
    int num_args = 0;

    strcpy(prev_str, "");
    // printf("len is %d\n", len);
    for(int i=0; i<len; i++)
    {
        // printf("    Char : %c\n", command[i]);
        if(!isspace(input[i])){
            str[arg_size++] = input[i];
        }
        else if( (i+1 < len && !isspace(input[i+1]) && arg_size >= 1) || (i+1 >= len && arg_size >= 1) ){
            str[arg_size] = '\0';
            arg_size = 0;
            // printf("        copying str : \"%s\" of len %d\n", str, arg_size);
            (*cmd_ptr)->argv[num_args] = (char*)malloc(sizeof(char)*(arg_size+10));

            if((*cmd_ptr)->argv[num_args] == NULL)
                error_exit("malloc agv[num_args] ");

            if(!strcmp(prev_str, "<")){
                strcpy((*cmd_ptr)->input_file, str);
                strcpy(prev_str, str);
                continue;
            }

            if(!strcmp(prev_str, ">") || !strcmp(prev_str, ">>")){
                strcpy((*cmd_ptr)->output_file, str);
                strcpy(prev_str, str);
                continue;
            }

            strcpy(prev_str, str);
            if(!strcmp(str, "<")){
                (*cmd_ptr)->input_redirect = true;
                continue;
            }

            if(!strcmp(str, ">")){
                (*cmd_ptr)->output_redirect = true;
                (*cmd_ptr)->output_append = false;
                continue;
            }

            if(!strcmp(str, ">>")){
                (*cmd_ptr)->output_redirect = true;
                (*cmd_ptr)->output_append = true;
                continue;
            }
            
            if((*cmd_ptr)->input_redirect){
                strcpy((*cmd_ptr)->input_file, str);
                continue;
            }

            if((*cmd_ptr)->output_redirect){
                strcpy((*cmd_ptr)->output_file, str);
                continue;
            }

            strcpy((*cmd_ptr)->argv[num_args], str);
            
            // printf("%d~~~~~~~~copied into %s~~~~~~~~~~~~\n",num_args,  args[num_args]);
            num_args++;
            
        }
    }
    (*cmd_ptr)->argv[num_args] = NULL;
    (*cmd_ptr)->argc = num_args;
    free(str);
}

void create_pipeline(char *input, cmd_pipeline *pipeline)
{
    char *token;
    int size = 0;   
    while((token = strsep(&input, "|")) != NULL){             
        struct command *cmd;
        initialize_cmd(&cmd);
        parse_command(&cmd, token);
        // print_command(cmd);
        size++;
        insert_command(pipeline, cmd);
    }
    // commands[size] =
    num_cmds = size;
    pipeline->num_of_cmds = num_cmds;
}

void print_command(struct command* cmd)
{
    printf("argc : %d, input_redirect : %d, output_redirect : %d, output_append: %d\n", cmd->argc, cmd->input_redirect, cmd->output_redirect, cmd->output_append);
    printf("input_file : %s, output_file %s\n", cmd->input_file, cmd->output_file);
    printf("argv : ");

    
    int j = 0;
    char *tmp = cmd->argv[j];
    while(tmp){
        printf("%s, ", tmp);
        tmp = cmd->argv[++j];
    }
    // printf("\n");
}

void print_commands(cmd_pipeline *pipeline){
    struct command *tmp;
    for(tmp = pipeline->first; tmp!=NULL; tmp = tmp->next){
        printf("\n=======================================\n");
        print_command(tmp);
        printf("\n=======================================\n");
    }
}

void remove_commands(cmd_pipeline *pipeline){
    struct command *tmp;
    for(tmp = pipeline->first; tmp!=NULL; tmp = tmp->next){
        free(tmp);
    }
    pipeline->first = NULL;
    pipeline->last = NULL;
    pipeline->num_of_cmds = 0;
}

void pipe_processes(struct command *cmd1, struct command *cmd2){
    int pipe_fd[2];

    if( pipe(pipe_fd) == -1)
        error_exit("pipe creation ");

    pid_t pid_proc1 = fork();

    if(pid_proc1 == -1)
        error_exit("fork pid_proc1 ");

    if(pid_proc1 == 0){
        printf("inside child1 : %s\n", cmd1->argv[0]);
        if(cmd2){
            if( close(pipe_fd[0]) == -1)
                error_exit("close fd proc1 ");

            if( dup2(pipe_fd[1], STDOUT_FILENO) == -1)
                error_exit("dup2 process 1 pipe ");
        }

        if(cmd1->input_redirect == true){
            int fd_input = open(cmd1->input_file, O_RDONLY);

            if(fd_input == -1)
                error_exit("input file open proc 1 ");
            
            if( dup2(fd_input, STDIN_FILENO) == -1)
                error_exit("dup2 process 1 stdin ");
        }

        if(cmd1->output_redirect && cmd1->output_append){
            int fd_output = open(cmd1->output_file, O_APPEND | O_WRONLY | O_CREAT, 0777);

            if(fd_output == -1)
                error_exit("output file open proc 1 ");
            
            if( dup2(fd_output, STDOUT_FILENO) == -1)
                error_exit("dup2 process 1 outfile ");
        }
        else if(cmd1->output_redirect){
            int fd_output = open(cmd1->output_file, O_TRUNC | O_WRONLY | O_CREAT, 0777);

            if(fd_output == -1)
                error_exit("output file open proc 1 ");

            if( dup2(fd_output, STDOUT_FILENO) == -1)
                error_exit("dup2 process 1 outfile ");
        }

        if( execvp(cmd1->argv[0], cmd1->argv) == -1)
            error_exit("execvp proc1 ");
    }

    if(cmd2 == NULL) return;
    pid_t pid_proc2 = fork();

    if(pid_proc2 == 0){
        
        printf("inside child2 : %s\n", cmd2->argv[0]);

        if( close(pipe_fd[1]) == -1)
            error_exit("close fd proc2 ");

        if( dup2(pipe_fd[0], STDIN_FILENO) == -1)
            error_exit("dup2 process 2 pipe ");
        
        if(cmd2->input_redirect == true){
            int fd_input = open(cmd2->input_file, O_RDONLY);

            if(fd_input == -1)
                error_exit("input file open proc 2 ");
            
            if( dup2(fd_input, STDIN_FILENO) == -1)
                error_exit("dup2 process 2 stdin ");
            
        }
        if(cmd2->output_redirect && cmd2->output_append){
            int fd_output = open(cmd2->output_file, O_APPEND | O_WRONLY | O_CREAT, 0777);
            
            if(fd_output == -1)
                error_exit("output file open proc 2 ");
            
            if( dup2(fd_output, STDOUT_FILENO) == -1)
                error_exit("dup2 process 2 outfile ");
        }
        else if(cmd2->output_redirect){
            int fd_output = open(cmd2->output_file, O_TRUNC | O_WRONLY | O_CREAT, 0777);

            if(fd_output == -1)
                error_exit("output file open proc 2 ");
            
            if( dup2(fd_output, STDOUT_FILENO) == -1)
                error_exit("dup2 process 2 outfile ");
        }
                 
        if( execvp(cmd2->argv[0], cmd2->argv) == -1)
            error_exit("execvp proc1 ");
    }
}


void execute_commands(cmd_pipeline *pipeline){
    struct command *tmp = pipeline->first;

    while(tmp != NULL){
        pipe_processes(tmp, tmp->next);
        tmp = tmp->next;
    }

    int status = 0;
    while(waitpid(-1, &status, 0) > 0);
}