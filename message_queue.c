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
int main(){
    /* Server Code */
    key_t key = ftok("./queue", 1234);  // creates the key for the message queues
    int q_id; // message queue id

    /* create and store message queue id */
    if((q_id = msgget(key, IPC_CREAT | 0644)) < 0){
        perror("server: msgget");
        exit(1);
    }

    /* declare message to send */
    struct msg_struct msg; // message struct
    msg.msgtype = 1; // must be greater than zero to allow for retrival
    char messages[3][10] = {"Hello", "Hello cat", "Hello you"}; // messages to send
    
    /* Parent Code */

    /* send messages to the queue in a loop */
    int i;
    for(i = 0; i < 3; i++){
        strncpy(msg.msgtext, messages[i], sizeof(messages[i]));
        if(msgsnd(q_id, &msg, sizeof(msg), 0) == 0){
            printf("Message sent: %s\n", messages[i]);
        }
        else{
            perror("Enqueue error: ");
            exit(1);
        }
    }
    /* End Server Code */

    /* Client Code */
    pid_t pid = fork(); // create a fork of the program

    if(pid < 0){
        perror("Error");
        exit(EXIT_FAILURE);
    }

    if(pid == 0){
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
}