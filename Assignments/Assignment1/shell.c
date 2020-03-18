#include "shell.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>        // for strsep
#include <ctype.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/shm.h>

void error_exit(char *err_msg)
{
    perror(err_msg);
    exit(EXIT_FAILURE);
}

void create_shared_memory(int size){
    int shmid;
    if((shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666)) == -1){
        error_exit("shmget ");
    }
    shm_id[shm_num++] = shmid;
}

void list_shmems(){
    printf("\n\n=========Shared memories list==========\n\n");
    printf("===   ID   === | ===   SIZE   ===\n");
    struct shmid_ds shm_info;
    for(int i = 0; i < shm_num; i ++){
        if( shmctl(shm_id[i], IPC_STAT, &shm_info) == -1){
            error_exit("shmctl ");
        }
        printf("===   %d   === | ===   %lu   ===\n", shm_id[i], shm_info.shm_segsz);
        char *buf = (char*)malloc(sizeof(char) * shm_info.shm_segsz);
        void *shm_addr = shmat(shm_id[i], NULL, 0);
        printf("Memory contents: %s\n", (char*)shm_addr);
    }
}

bool is_valid_shm(int num){
    
    for(int i = 0; i < shm_num; i++){
        if(shm_id[i] == num)
            return true;
    }
    return false;
    
}
bool is_str_number(char *str){
    // printf("checking is number (%s)\n", str);
    int len = strlen(str);
    for(int i = 0; i < len; i++){
        if(!isdigit(str[i]))
            return false;
    }

    return true;
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
        size++;

        if(cmd->input_redirect && is_str_number(cmd->input_file)){
            printf("shmin\n");
            struct command *shm_cmd;
            initialize_cmd(&shm_cmd);
            shm_cmd->argv[0] = "./shm";
            
            char type[ARG_LENGTH];
            shm_cmd->argv[1] = (char*)malloc(sizeof(char) * ( strlen(cmd->input_file) + 1 ) );
            perror("hi1");
            strcpy(shm_cmd->argv[1], cmd->input_file);
            perror("hi2");
            shm_cmd->argv[2] = "0";

            shm_cmd->argv[3] = "10";

            size++;
            insert_command(pipeline, shm_cmd);
        }


        insert_command(pipeline, cmd);


        if(cmd->output_redirect && is_str_number(cmd->output_file)){
            printf("shmout\n");
            struct command *shm_cmd;
            initialize_cmd(&shm_cmd);
            shm_cmd->argv[0] = "./shm";
            
            char type[ARG_LENGTH];
            shm_cmd->argv[1] = (char*)malloc(sizeof(char) * ( strlen(cmd->output_file) + 1 ) );
            perror("hi3");
            strcpy(shm_cmd->argv[1], cmd->output_file);
            perror("hi4");
            shm_cmd->argv[2] = "0";

            shm_cmd->argv[3] = "11";

            size++;
            insert_command(pipeline, shm_cmd);
        }
        
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
    if(!pipeline) return;
    for(tmp = pipeline->first; tmp!=NULL; tmp = tmp->next){
        free(tmp);
    }
    pipeline->first = NULL;
    pipeline->last = NULL;
    pipeline->num_of_cmds = 0;
}


void close_all_pipes(int pipe_fd[][2], int count){

    for(int i = 0; i < count; i++){
        close(pipe_fd[i][0]);
        close(pipe_fd[i][1]);
    }

}
void execute_commands(cmd_pipeline *pipeline){
    struct command *tmp = pipeline->first;
    int count = pipeline->num_of_cmds;
    int pipe_fd[count-1][2];
    for(int i = 0; i < count - 1; i++){
        if( pipe(pipe_fd[i]) == -1)
            error_exit("pipe creation ");
    }
    int i;
    struct command *cmd;
    for(i = 0, cmd = pipeline->first; i < count, cmd != NULL; i++, cmd = cmd->next){
        // printf("loop iteration num : %d\n", i);
        if(i < count-1)
            printf("\n===========pipe[%d] : read_fd = %d, write_fd = %d===========\n", i, pipe_fd[i][0], pipe_fd[i][1]);
        
        if(!strcmp(cmd->argv[0], "shmcreate" ) ){
            int size = atoi(cmd->argv[1]);
            create_shared_memory(size);
            break;
        }
        else if(!strcmp(cmd->argv[0], "shmls" ) ){
            list_shmems();
            break;
        }
        else if(!strcmp(cmd->argv[0], "tcpcreate" ) ){
            
            break;
        }
        pid_t pid = fork();

        if(pid == -1)
            error_exit("fork");

        if(pid == 0){
            printf("\n===========process[%d] pid: %d===========\n", i, getpid());            
            
            if( i != 0){
                // printf("Attaching read of pipe %d to process %d", i-1, getpid());
                if( dup2(pipe_fd[i-1][0], STDIN_FILENO) == -1)
                    error_exit("dup2 pipe read ");
            }

            if( i != count -1){
                // printf("Attaching write of pipe %d to process %d", i, getpid());
                if( dup2(pipe_fd[i][1], STDOUT_FILENO) == -1)
                    error_exit("dup2 pipe write ");
            }

            if(cmd->input_redirect == true && !is_str_number(cmd->input_file)){
                int fd_input;
                
                fd_input = open(cmd->input_file, O_RDONLY);

                // printf("input redirection allowed on fd :  %d\n", fd_input);

                if(fd_input == -1)
                    error_exit("input file open ");
                
                if( dup2(fd_input, STDIN_FILENO) == -1)
                    error_exit("dup2 stdin ");
                
                if( close(fd_input) == -1)
                    error_exit("close input redirection proc ");
            }

            if(cmd->output_redirect && cmd->output_append && !is_str_number(cmd->output_file)){
                int fd_output;
                
                fd_output = open(cmd->output_file, O_APPEND | O_WRONLY | O_CREAT, 0777);

                // printf("output redirection allowed on fd :  %d\n", fd_output);

                if(fd_output == -1)
                    error_exit("output file open");
                
                if( dup2(fd_output, STDOUT_FILENO) == -1)
                    error_exit("dup2 process outfile ");
                
                if( close(fd_output) == -1)
                    error_exit("close output redirection");
            }
            else if(cmd->output_redirect && !is_str_number(cmd->output_file)){
                int fd_output = open(cmd->output_file, O_TRUNC | O_WRONLY | O_CREAT, 0777);

                // printf("output redirection allowed on fd :  %d\n", fd_output);

                if(fd_output == -1)
                    error_exit("output file open");

                if( dup2(fd_output, STDOUT_FILENO) == -1)
                    error_exit("dup2 outfile ");
                
                if( close(fd_output) == -1)
                    error_exit("close output redirection");
            }

            close_all_pipes(pipe_fd, count-1);

            if( execvp(cmd->argv[0], cmd->argv) == -1){
                error_exit("execvp");
            }
   
        }
    }

    close_all_pipes(pipe_fd, count-1);
    // int status = 0;
    while(waitpid(-1, NULL , 0) > 0);
}