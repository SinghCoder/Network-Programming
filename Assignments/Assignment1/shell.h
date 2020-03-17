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

typedef struct {
    struct command *first;
    struct command *last;
    int num_of_cmds;
}cmd_pipeline;

int num_cmds;
char *input;
// char commands[NUM_OF_CMDS][NUM_OF_ARGS][ARG_LENGTH];
void error_exit(char *err_msg);
void create_pipeline(char *input, cmd_pipeline *pipeline);
void parse_command(struct command **cmd_ptr, char *input);
void print_command(struct command *cmd);
void initialize_cmd(struct command **cmd_ptr);
void insert_command(cmd_pipeline *pipeline, struct command *cmd);
void print_commands(cmd_pipeline *pipeline);
void remove_commands(cmd_pipeline *pipeline);
void execute_commands(cmd_pipeline *pipeline);
void pipe_processes(struct command *cmd1, struct command *cmd2);
#endif
