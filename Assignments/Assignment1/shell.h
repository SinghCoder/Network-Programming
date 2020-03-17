#ifndef SHELL_H
#define SHELL_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define NUM_OF_CMDS 20
#define NUM_OF_ARGS 20
#define ARG_LENGTH 50

// typedef enum {false, true} bool;

struct command{
    int argc;
    char *argv[NUM_OF_ARGS];
    bool input_redirect;
    bool output_redirect;
    bool output_append;
    char input_file[ARG_LENGTH];
    char output_file[ARG_LENGTH];
    struct command *next;
};

int num_cmds;
char *input;
// char commands[NUM_OF_CMDS][NUM_OF_ARGS][ARG_LENGTH];
void error_exit(char *err_msg);
void parse_input(char *input);
void parse_command(struct command **cmd_ptr, char *input);
void print_command(struct command *cmd);
void initialize_cmd(struct command **cmd_ptr);

#endif
