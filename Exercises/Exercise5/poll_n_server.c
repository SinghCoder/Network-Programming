#include "poll_n_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include "poll.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char *argv[])
{
    if(argc < 2){
        printf("Usage: %s N\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int max_cli_cnt = atoi(argv[1]); 

    struct sockaddr_in client_addr, server_addr;

    int listen_sockfd, conn_sockfd, client_sockfd;

    struct pollfd poll_fds[max_cli_cnt + 1]; // 1 for listen_sockfd

    int curr_cli_count = 0;

    for(int i = 0; i < max_cli_cnt; i++){
        poll_fds[i].fd = -1;
        poll_fds[i].events = POLLIN;
    }

    listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    poll_fds[max_cli_cnt].fd = listen_sockfd;
    poll_fds[max_cli_cnt].events = POLLIN;
    bzero(&server_addr, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(listen_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(listen_sockfd, MAX_QUEUE_SIZE);

    int poll_status;
    int num_ready = 0;
    char buffer[MAX_BFR_SIZE];

    for( ; ; ){
        // sleep(1);
        printf("Polling \n"); 
        poll_status = poll(poll_fds, max_cli_cnt + 1, TIMEOUT_MS);
        printf("Poll successful\n");
        if(poll_status == -1){
            perror("poll");
            exit(EXIT_FAILURE);
        }

        if(poll_status == 0){
            printf("Timeout occured\n");
            return 0;
        }

        num_ready = poll_status;
        int cli_len = sizeof(client_addr);
        int i;

        if(poll_fds[max_cli_cnt].revents & POLLIN){  // listening socket is readable
            printf("Listening socket is readable\n");
            conn_sockfd = accept(listen_sockfd, (struct sockaddr *)&client_addr, &cli_len);

            if(conn_sockfd < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            if(curr_cli_count == max_cli_cnt){
                printf("Number of clients exceeded\n");
                close(conn_sockfd);
            }
            else{                
                curr_cli_count++;
                int already_present = 0;

                for(i = 0; i < max_cli_cnt; i++){    // already present poll_fd
                    if(poll_fds[i].fd == conn_sockfd){
                        already_present = 1;
                        break;            
                    }
                }

                if(!already_present){
                    for(i = 0; i < max_cli_cnt; i++){
                        if(poll_fds[i].fd == -1){
                            printf("Adding a connection socket at %d location in the array\n", i);
                            poll_fds[i].fd = conn_sockfd;
                            break;
                        }
                    }
                }
            }
        }

        // if(--num_ready <= 0){
        //     continue;   // no more readable clients
        // }

        for(i = 0; i < max_cli_cnt; i++){
            if( (poll_fds[i].fd != -1) && (poll_fds[i].revents & POLLIN)){
                /**
                 * @brief The client exists at this fd, and the fd is readable.
                 */
                // Read from this client and send to all other
                printf("poll_fd %d is readable, recieving data from it\n", poll_fds[i].fd);
                int n = read(poll_fds[i].fd, buffer, MAX_BFR_SIZE);
                if(n < 0){
                    perror("read from poll_fd");
                    exit(EXIT_FAILURE);
                }
                printf(" Recieved data from client %s\n", buffer);
                if(n == 0){ // this client has closed the connection
                    printf("client with fd %d closed the connection\n",poll_fds[i].fd);
                    close(poll_fds[i].fd);
                    curr_cli_count--;
                    poll_fds[i].fd = -1;
                }
                else{   // Data received, now send
                    for(int j = 0; j < max_cli_cnt; j++){
                        if((j != i) && (poll_fds[j].fd != -1)){
                            n = send(poll_fds[j].fd, buffer, MAX_BFR_SIZE, 0);
                            if(n < 0){
                                perror("send");
                            }
                            else{
                                printf("sent %s to %d client\n", buffer,j);
                            }
                        }
                    }
                }
            }   // end of if
            // if(--num_ready == 0){
            //     /**
            //      * @brief No more readable clients
            //      */
            //     break;  // No more clients here, poll again
            // }
        }   // End of loop checking readability 
        // sleep(10);
    }   // Main for loop for polling

    return 0;
}