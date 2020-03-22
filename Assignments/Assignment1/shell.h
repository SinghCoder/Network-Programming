#ifndef SHELL_H
#define SHELL_H
#include <stdlib.h>
#include <stdio.h>               
#include <stdbool.h>             // for true/false

#define NUM_OF_CMDS 20           // max num of commands allowed in a pipeline
#define NUM_OF_ARGS 20           // max num of arguements per command
#define NUM_OF_SHM 100           // max num of shared memory objects 
#define NUM_OF_TCP_SOCKS 100     // max num of active tcp connections
#define ARG_LENGTH 50            // max size of an arguement
#define BUFFERSIZE 1000          // max data sent/received over tcp connection
#define SHM_READ 10              // code to symbolise shared memory read operation
#define SHM_WRITE 11             // code to symbolise shared memory write operation
#define SOCK_READ 10             // code to symbolise read operation over tcp socket
#define SOCK_WRITE 11            // code to symbolise write operation over tcp socket
/**
 * @brief Structure storing details of a command
 * 
 */
struct command{
    int argc;   // number of arguements to command
    char *argv[NUM_OF_ARGS];    // list of arguement
    bool input_redirect;        // is input redirected
    bool output_redirect;       // is output redirected
    bool output_append;         // is output to be appended
    char input_file[ARG_LENGTH];// input file in case input redirected
    char output_file[ARG_LENGTH];// output file in case output is redirected
    struct command *next;       // next command in pipeline
};
/**
 * @brief structure storing pipeline
 * 
 */
typedef struct {
    struct command *first;      // first command in pipeline
    struct command *last;       // last command in pipeline
    int num_of_cmds;            // number of cmds in pipeline
}cmd_pipeline;

int num_cmds;                       // number of commands
char *input;                        // input typed at command line
int shm_id[NUM_OF_SHM];             // array of shared memories created by shmcreate cmd
int tcp_sock_id[NUM_OF_TCP_SOCKS];  // array of tcp connection fds opened up by tcpcreate cmd
int shm_num;                        // number of shared memories existing
int tcp_sock_num;                   // number of active tcp connections
/**
 * @brief Display error message on error and exit
 * 
 */
void error_exit(char *err_msg);
/**
 * @brief Parse input commands separated by pipes and create a pipeline
 * 
 * @param input - String entered at command line
 * @param pipeline - Linked lists of commands
 */
void create_pipeline(char *input, cmd_pipeline *pipeline);
/**
 * @brief - Given a command containing redirection operators, parse it and create a node in linked list
 * 
 * @param cmd_ptr - Command struct to be formed
 * @param input - Command containing redirection operators
 */
void parse_command(struct command **cmd_ptr, char *input);
/**
 * @brief Give initial values to fields of command structure
 * 
 * @param cmd_ptr - Structure to be initialized
 */
void initialize_cmd(struct command **cmd_ptr);
/**
 * @brief Insert the given command to end of pipeline linked list
 * 
 */
void insert_command(cmd_pipeline *pipeline, struct command *cmd);
/**
 * @brief Helper function to print a command
 * 
 */
void print_command(struct command *cmd);
/**
 * @brief Helper function to print details of all commands in pipeline
 * 
 * @param pipeline 
 */
void print_commands(cmd_pipeline *pipeline);
/**
 * @brief To remove commands from pipeline, free the memory
 * 
 */
void remove_commands(cmd_pipeline *pipeline);
/**
 * @brief Execute commands in the pipeline by forking necessary processes and creating pipes between them
 * 
 */
void execute_commands(cmd_pipeline *pipeline);
/**
 * @brief Helper function to close all pipes opened up in parent or unnecessary pipes to avoid blocking
 * 
 * @param pipe_fd - Array of pipes
 * @param count - Number of pipes
 */
void close_all_pipes(int pipe_fd[][2], int count);
/**
 * @brief Create a shared memory of given size
 * 
 */
void create_shared_memory(int size);
/**
 * @brief List all shared memories created
 * 
 */
void list_shmems();
/**
 * @brief Check if a string is number
 * 
 */
bool is_str_number(char *str);
/**
 * @brief Create a tcp connection at given ip and port by opening a socket
 * 
 */
void create_tcp_coxn(char *ip, int port);
/**
 * @brief List all active tcp connections
 * 
 */
void list_tcp_coxns();
#endif
