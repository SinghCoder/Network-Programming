#include "shell.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>        // for strsep
#include <ctype.h>

void error_exit(char *err_msg)
{
    perror(err_msg);
    exit(EXIT_FAILURE);
}

void initialize_cmd(struct command **cmd_ptr){
    *cmd_ptr = (struct command*)malloc(sizeof(struct command));
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

void parse_command(struct command **cmd_ptr, char *input)
{
    int len = strlen(input);
    char *str = (char*)malloc(sizeof(char)*(len+1));
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

void parse_input(char *input)
{
    char *token;
    int size = 0;
    struct command *cmd;
    initialize_cmd(&cmd);
    while((token = strsep(&input, "|")) != NULL){             
        parse_command(&cmd, token);
        print_command(cmd);
        size++;
        initialize_cmd(&cmd);
    }
    // commands[size] =
    num_cmds = size;
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
    printf("\n");
}