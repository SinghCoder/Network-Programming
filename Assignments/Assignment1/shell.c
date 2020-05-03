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
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

void error_exit(char *err_msg)
{
    perror(err_msg);
    exit(EXIT_FAILURE);
}

void create_shared_memory(int size){
    int shmid;
    if((shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666)) == -1){    // create with a random key and give access permissions to user
        error_exit("shmget ");
    }
    shm_id[shm_num++] = shmid;  // append id of created shmem

    void *shm_addr = shmat(shmid, NULL, 0);
    memset(shm_addr, 0, size);      //initialize shmem to all 0s
    shmdt(shm_addr);
}

void list_shmems(){
    printf("\n\n=========Shared memories list==========\n\n");
    printf("===   ID   === | ===   SIZE   === | ===   DATA SIZE  === |\n");
    
    struct shmid_ds shm_info;
    
    for(int i = 0; i < shm_num; i ++){
        
        if( shmctl(shm_id[i], IPC_STAT, &shm_info) == -1){  // get size of shared memory
            error_exit("shmctl ");
        }
    
        void *shm_addr = shmat(shm_id[i], NULL, 0);

        printf("===   %d   === | ===   %lu   === | ===   %d   === |\n", shm_id[i], shm_info.shm_segsz, *(int *)shm_addr);
        
        char *buf = (char*)malloc(sizeof(char) * shm_info.shm_segsz);
        
        printf("Memory contents: %s\n", (char*)( shm_addr + sizeof(int) ) );
    }
}

bool is_valid_shm(int num){     // does num correspond to a valid shmem id
    
    for(int i = 0; i < shm_num; i++){
        if(shm_id[i] == num)
            return true;
    }

    return false;
    
}

bool is_valid_tcp_coxn(int num){     // does num correspond to a valid tcp connection fd
        
    for(int i = 0; i < tcp_sock_num; i++){
        if(tcp_sock_id[i] == num)
            return true;
    }
    return false;
    
}

bool is_str_number(char *str){
    int len = strlen(str);

    for(int i = 0; i < len; i++){
        if(!isdigit(str[i]))
            return false;
    }

    return true;
}

void create_tcp_coxn(char *ip, int port){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);   // create a tcp connection at given ip,port
    
    if(sockfd == -1)
        error_exit("socket creation ");
    
    tcp_sock_id[tcp_sock_num++] = sockfd;
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    int connect_status = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if(connect_status < 0)
        error_exit("Socket connection");
}

void list_tcp_coxns(){
    printf("\n\n=========TCP connections list==========\n\n");
    printf("===   ID   === | ===   IP   === | ===   Port   ===\n");
    
    struct sockaddr_in peeraddr;
    memset(&peeraddr, 0, sizeof(peeraddr));
    socklen_t peeraddrlen = sizeof(peeraddr);
    
    for(int i = 0; i < tcp_sock_num; i ++){
        getpeername(tcp_sock_id[i], (struct sockaddr*)&peeraddr, &peeraddrlen);
        printf("===   %d   === | ===   %s   === | ===   %d   ===\n", tcp_sock_id[i], inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
    }
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
    char *str = (char*)malloc(sizeof(char)*(len+1));    // to store cuyrrent arguement
    
    if(str == NULL)
        error_exit("malloc str");

    char prev_str[ARG_LENGTH];      // to store previous arguement
    int arg_size = 0;               // size of current arguement
    int num_args = 0;               // num of args present in cmd

    strcpy(prev_str, "");
    for(int i=0; i<len; i++)    //read char by char
    {
        if(!isspace(input[i])){     // if not space, store the character
            str[arg_size++] = input[i];
        }

        else if( (i+1 < len && !isspace(input[i+1]) && arg_size >= 1) || (i+1 >= len && arg_size >= 1) ){
            // if next character exists and is not space and the arguement contains atleast one character
            // or no next character but the arguement contains atleast one character
            str[arg_size] = '\0';   // recognized an arguement
            arg_size = 0;           // arg length nullified

            if(!strcmp(prev_str, "<")){     // if previously input redirection operator was recognized, store file
                strcpy((*cmd_ptr)->input_file, str);
                strcpy(prev_str, str);
                continue;
            }

            if(!strcmp(prev_str, ">") || !strcmp(prev_str, ">>")){      // if previously output redirection was recognized, store file
                strcpy((*cmd_ptr)->output_file, str);
                strcpy(prev_str, str);
                continue;
            }

            strcpy(prev_str, str);      // update prev str
            
            if(!strcmp(str, "<")){
                (*cmd_ptr)->input_redirect = true;      //mark input redirection
                continue;
            }

            if(!strcmp(str, ">")){
                (*cmd_ptr)->output_redirect = true;
                (*cmd_ptr)->output_append = false;      // mark output redirection
                continue;
            }

            if(!strcmp(str, ">>")){
                (*cmd_ptr)->output_redirect = true;     // mark output append
                (*cmd_ptr)->output_append = true;
                continue;
            }
            
            (*cmd_ptr)->argv[num_args] = (char*)malloc(sizeof(char)*(arg_size+10));     

            if((*cmd_ptr)->argv[num_args] == NULL)
                error_exit("malloc agv[num_args] ");
                
            strcpy((*cmd_ptr)->argv[num_args], str);    // copy the arguement
            
            num_args++;
            
        }
    }

    (*cmd_ptr)->argv[num_args] = NULL;      // mark the end of arguement vector by a NULL pointer
    (*cmd_ptr)->argc = num_args;
    free(str);
}

void create_pipeline(char *input, cmd_pipeline *pipeline)
{
    char *token;
    int size = 0;   

    while((token = strsep(&input, "|")) != NULL){       // tokenize by | operator, u get one command
        struct command *cmd;
        initialize_cmd(&cmd);
        
        parse_command(&cmd, token);        // fill command structure
        
        size++;

        if(cmd->input_redirect && is_str_number(cmd->input_file)){      // read opern to shmem/tcp

            int inp_fd = atoi(cmd->input_file);

            if(is_valid_shm(inp_fd)){   // read from shmem

                struct command *shm_cmd;
                initialize_cmd(&shm_cmd);   // insert a dummy process which reads data from shmem and forwards ahead
            
                shm_cmd->argv[0] = "./shm";
                
                char type[ARG_LENGTH];
                shm_cmd->argv[1] = (char*)malloc(sizeof(char) * ( strlen(cmd->input_file) + 1 ) );

                strcpy(shm_cmd->argv[1], cmd->input_file);

                char opern[5];
                sprintf(opern, "%d", SHM_READ);
                shm_cmd->argv[2] = (char*)malloc(sizeof(char) * strlen(opern));
                strcpy(shm_cmd->argv[2], opern);    // perform a shmem read operation

                size++;
                insert_command(pipeline, shm_cmd);
            }
            else if(is_valid_tcp_coxn(inp_fd)){
                struct command *tcp_cmd;
                initialize_cmd(&tcp_cmd);    // insert a dummy process which reads data from socket and forwards ahead
                tcp_cmd->argv[0] = "./tcp_client";
                
                char type[ARG_LENGTH];
                tcp_cmd->argv[1] = (char*)malloc(sizeof(char) * ( strlen(cmd->input_file) + 1 ) );

                strcpy(tcp_cmd->argv[1], cmd->input_file);
                
                char opern[5];
                sprintf(opern, "%d", SOCK_READ);

                tcp_cmd->argv[2] = (char*)malloc(sizeof(char) * strlen(opern));
                strcpy(tcp_cmd->argv[2], opern);    // perform a tcp sockt read operation

                size++;
                insert_command(pipeline, tcp_cmd);
            }
            else{
                printf("id is neither a valid tcp connection nor valid shm\n\n");
                exit(EXIT_FAILURE);
            }
        }


        insert_command(pipeline, cmd);


        if(cmd->output_redirect && is_str_number(cmd->output_file)){
            int out_fd = atoi(cmd->output_file);
            
            if(is_valid_shm(out_fd)){
                struct command *shm_cmd;
                initialize_cmd(&shm_cmd);
                shm_cmd->argv[0] = "./shm";
                
                char type[ARG_LENGTH];

                shm_cmd->argv[1] = (char*)malloc(sizeof(char) * ( strlen(cmd->output_file) + 1 ) );
                
                strcpy(shm_cmd->argv[1], cmd->output_file);

                char opern[5];
                sprintf(opern, "%d", SHM_WRITE);

                shm_cmd->argv[2] = (char*)malloc(sizeof(char) * strlen(opern));
                strcpy(shm_cmd->argv[2], opern);    // perform a shmem write operation

                shm_cmd->argv[3] = (cmd->output_append == true) ? "APPEND" : "OVERWRITE";

                size++;
                insert_command(pipeline, shm_cmd);
            }
            else if(is_valid_tcp_coxn(out_fd)){
                struct command *tcp_cmd;
                initialize_cmd(&tcp_cmd);
                tcp_cmd->argv[0] = "./tcp_client";
                
                char type[ARG_LENGTH];
                tcp_cmd->argv[1] = (char*)malloc(sizeof(char) * ( strlen(cmd->output_file) + 1 ) );
                
                strcpy(tcp_cmd->argv[1], cmd->output_file);

                char opern[5];
                sprintf(opern, "%d", SOCK_WRITE);

                tcp_cmd->argv[2] = (char*)malloc(sizeof(char) * strlen(opern));
                strcpy(tcp_cmd->argv[2], opern);    // perform a tcp sockt write operation

                size++;
                insert_command(pipeline, tcp_cmd);
            }
            else{
                printf("id is neither a valid tcp connection nor valid shm\n\n");
                exit(EXIT_FAILURE);
            }
        }
        
    }
    
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

    int pipe_fd[count-1][2];    // create n-1 pipes for n processes

    for(int i = 0; i < count - 1; i++){
        if( pipe(pipe_fd[i]) == -1)
            error_exit("pipe creation ");
    }
    int i;
    struct command *cmd;

    for(i = 0, cmd = pipeline->first; i < count, cmd != NULL; i++, cmd = cmd->next){

        if(i < count-1) // num of pipes is n-1
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
            create_tcp_coxn(cmd->argv[1], atoi(cmd->argv[2]));    //argv[1] = IP, argv[2] = port
            break;
        }

        else if(!strcmp(cmd->argv[0], "tcpls" ) ){
            list_tcp_coxns();
            break;
        }

        pid_t pid = fork();

        if(pid == -1)
            error_exit("fork");

        if(pid == 0){
            printf("\n===========process[%d] pid: %d===========\n", i, getpid());            
            
            if( i != 0 ){   // attach read end of stdin to pipe for all processes except first
                if( dup2(pipe_fd[i-1][0], STDIN_FILENO) == -1)
                    error_exit("dup2 pipe read ");
            }

            if( i != count -1){ // attach write end of stdout to pipe for all processes except last
                if( dup2(pipe_fd[i][1], STDOUT_FILENO) == -1)
                    error_exit("dup2 pipe write ");
            }

            if(cmd->input_redirect == true && !is_str_number(cmd->input_file)){     // input redirection to a file
                int fd_input;
                
                fd_input = open(cmd->input_file, O_RDONLY);

                if(fd_input == -1)
                    error_exit("input file open ");
                
                if( dup2(fd_input, STDIN_FILENO) == -1)     // attach the file descriptor to stdin
                    error_exit("dup2 stdin ");
                
                if( close(fd_input) == -1)
                    error_exit("close input redirection proc ");
            }

            if(cmd->output_redirect && cmd->output_append && !is_str_number(cmd->output_file)){ // output append to a file
                int fd_output;
                
                fd_output = open(cmd->output_file, O_APPEND | O_WRONLY | O_CREAT, 0777);    // open file in append mode

                if(fd_output == -1)
                    error_exit("output file open");
                
                if( dup2(fd_output, STDOUT_FILENO) == -1)   // attach fd to stdout
                    error_exit("dup2 process outfile ");
                
                if( close(fd_output) == -1)
                    error_exit("close output redirection");
            }
            else if(cmd->output_redirect && !is_str_number(cmd->output_file)){  // output redirect to a file
                int fd_output = open(cmd->output_file, O_TRUNC | O_WRONLY | O_CREAT, 0777); 

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
        else{
            
            if( i-1 > 0 )
                close(pipe_fd[i-1][0]);     // close necessary pipes so that corresponding child can exit
            
            close(pipe_fd[i][1]);
            
            if( wait(NULL) == -1){          // wait for this child to exit
                error_exit("wait pid ");
            }
        }
    }

    close_all_pipes(pipe_fd, count-1);
}