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

    int num_of_clients = atoi(argv[1]); 

    struct sockaddr_in client_addr, server_addr;

    int listen_sockfd, conn_sockfd, client_sockfd;

    struct pollfd poll_fds[num_of_clients + 1]; // 1 for listen_sockfd

    int curr_cli_count = 0;

    for(int i = 0; i < num_of_clients; i++){
        poll_fds[i].fd = -1;
        poll_fds[i].events = POLLIN;
    }

    listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    poll_fds[num_of_clients].fd = listen_sockfd;
    poll_fds[num_of_clients].events = POLLIN;
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
        poll_status = poll(poll_fds, num_of_clients + 1, TIMEOUT_MS);

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
        if(poll_fds[num_of_clients].revents & POLLIN)  // listening socket is readable
            conn_sockfd = accept(listen_sockfd, (struct sockaddr *)&client_addr, &cli_len);
        
        int i;

        for(i = 0; i < num_of_clients; i++){
            if(poll_fds[i].fd == conn_sockfd)
                break;
            if(poll_fds[i].fd == -1){
                poll_fds[i].fd = conn_sockfd;
                break;
            }
        }

        if(i == num_of_clients){
            printf("Number of clients exceeded %d\n", num_of_clients);
            exit(EXIT_FAILURE);
        }

        if(--num_ready <= 0){
            continue;   // no more readable clients
        }

        for(i = 0; i < num_of_clients; i++){
            if( (poll_fds[i].fd != -1) && (poll_fds[i].revents & POLLIN)){
                /**
                 * @brief The client exists at this fd, and the fd is readable.
                 */
                // Read from this client and send to all other
                int n = read(poll_fds[i].fd, buffer, MAX_BFR_SIZE);
                if(n == 0){ // this client has closed the connection
                    close(poll_fds[i].fd);
                    poll_fds[i].fd = -1;
                }
                else{   // Data received, now send
                    for(int j = 0; j < num_of_clients; j++){
                        if(j != i){
                            send(poll_fds[i].fd, buffer, MAX_BFR_SIZE, 0);
                        }
                    }
                }
            }   // end of if
            if(--num_ready == 0){
                /**
                 * @brief No more readable clients
                 */
                break;  // No more clients here, poll again
            }
        }   // End of loop checking readability 
    }   // Main for loop for polling

    return 0;
}