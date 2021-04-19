#ifndef SYSOPY_MY_MESSAGES_H
#define SYSOPY_MY_MESSAGES_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include "utils.h"

#define MAX_MESSAGE_LEN 256

enum TYPES{STOP = 1, INIT, DISCONNECT, CONNECT, LIST, ALL_TYPES};
enum CLIENT_STATUS{NOT_CONNECTED = -5, BUSY, READY};



#define ID 'I'
#define CLIENT_ID ((char) (getpid() % 'a'))
#define HOME getenv("HOME")
#define MAX_CLIENTS 10
#define SERVER_ID (MAX_CLIENTS + 1)
#define NO_FREE_INDEX (-1)
#define NO_SUCH_ID (-1)

char *status_to_str(int client_status){
    switch (client_status) {
        case NOT_CONNECTED:
            return (char*) "not connected";
        case BUSY:
            return (char*) "busy";
        case READY:
            return (char*) "ready";
        default:
            return (char*) dunno;
    }
}

typedef struct{
    long mtype;
    int sender_id;
    int client_id;
    int queue;
}message;

size_t message_size = sizeof(message) - sizeof (long );

int create_queue(char* path, char id, int flag){
    key_t key = ftok(path, id);

    if (key == -1){
        eprint("cannot get key for queue, errno: %s", strerror(errno));
    }

    int q;
    if((q = msgget(key, flag)) == -1) {
        eprint("cannot create queue, errno: %s", strerror(errno));
    }
    return q;
}

#define send_message(queue, m, flags) { \
if (msgsnd(queue, &m, message_size, flags) == -1){ \
    eprint("cannot send message, errno: %s", strerror(errno));}}

int receive_message(int queue, long mtype, message* m){
    if(msgrcv(queue, m, message_size, mtype, 0) == -1){
        eprint("cannot receive message, errno: %s", strerror(errno));
    }

    return 0;
}
void close_queue(int q){
    msgctl(q, IPC_RMID, NULL);
}


#endif //SYSOPY_MY_MESSAGES_H
