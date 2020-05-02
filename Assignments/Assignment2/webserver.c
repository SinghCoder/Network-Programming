#include "webserver.h"

void *processClients( void *input){

    int msqid = *(int *)input;
    msgqbuf msg;

    while(true){
        Msgrcv(msqid, &msg, sizeof(msg), /* msgtyp = */ 0, /* msgflags = */ 0);

        clientInfo *clientPtr = searchTable(clientInfoTable, msg.sockfd);

        if(clientPtr == NULL){
            clientPtr = (clientInfo *) malloc(sizeof(clientInfo));
            clientPtr->currState = READING_REQUEST;
            clientPtr->friptr = NULL;
            clientPtr->froptr = NULL;
            clientPtr->toiptr = NULL;
            clientPtr->tooptr = NULL;

            insertInTable(clientInfoTable, msg.sockfd, clientPtr);
            continue;
        }

        state clientState = clientPtr->currState;

        switch(clientState){
            
            case READING_REQUEST:
            {

            }
            break;

            case HEADER_PARSING:
            {

            }
            break;

            case READING_DISKFILE:
            {

            }
            break;

            case WRITING_HEADER:
            {

            }
            break;

            case WRITING_BODY:
            {

            }
            break;

            case DONE:
            {

            }
            break;

            default:
            {
                printf("It shoulf not have come here!! Something's wrong..\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

int main(){

    initTable(clientInfoTable);

}