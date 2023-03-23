#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

/* declare a message structure */
struct msg_struct {
    long int msgtype; // assigned by program
    char msgtext[100]; // message up to 2048 bytees
};

extern int errno; // indicate error
int main(int argc, char *argv[]){
    printf("client executed");

    key_t key = ftok("./queue", 1234);  // creates the key for the message queues
    int q_id; // message queue id

    /* create and store message queue id */
    if((q_id = msgget(key, IPC_CREAT | 0644)) < 0){
        perror("server: msgget");
        exit(1);
    }

    /* declare message to receive */
    struct msg_struct msg; // message struct
    msg.msgtype = 1; // must be greater than zero to allow for retrival

    // retrieve messages in the queue
    int j;
    for(int j = 0; j < 3; j++){
        // retrieve messages on queue
        if(msgrcv(q_id, &msg, sizeof(msg), 1, IPC_NOWAIT) > 0){
            printf("Message recieved: %s\n", msg.msgtext);
        } else {
            perror("Dequeue error: ");
            exit(1);
        }
    }
}        