#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/* declare a message structure */
struct msg_struct {
    long int msgtype; // assigned by program
    char msgtext[2048]; // message up to 2048 bytees
}

extern int errno; // indicate error
int main(){
    key_t key = ftok("./queue", 1234);  // creates the key for the message queues
    int q_id; // message queue id

    /* create and store message queue id */
    if((q_id = msgget(key, IPC_CREAT | 0644)) < 0){
        perror("server: msgget");
        exit(1);
    }

    /* declare message to send */
    struct msg msg_struct; // message to send
    msg.msgtype = 1; // must be greater than zero to allow for retrival   

}