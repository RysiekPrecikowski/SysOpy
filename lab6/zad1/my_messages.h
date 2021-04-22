#ifndef SYSOPY_MY_MESSAGES_H
#define SYSOPY_MY_MESSAGES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <signal.h>
#include <fcntl.h>

#include "utils.h"



enum TYPES {
    STOP = 1, INIT, DISCONNECT, CONNECT, LIST, CHAT, ALL_TYPES
};

enum CLIENT_STATUS {
    NOT_CONNECTED = -5, BUSY, READY
};

char *status_to_str(int client_status) {
    switch (client_status) {
        case NOT_CONNECTED:
            return (char *) "not connected";
        case BUSY:
            return (char *) "busy";
        case READY:
            return (char *) "ready";
        default:
            return (char *) dunno;
    }
}

#define ID 'I'
#define CLIENT_ID ((char) (getpid() % 'a'))
#define HOME getenv("HOME")

#define MAX_MESSAGE_LEN 256
#define MAX_CLIENTS 10
#define SERVER_ID (MAX_CLIENTS + 1)

#define NO_FREE_INDEX (-1)
#define NO_SUCH_ID (-1)


typedef struct {
    long mtype;
    int sender_id;
    int client_id;
    int queue;
    char message[MAX_MESSAGE_LEN];
} message;

size_t message_size = sizeof(message) - sizeof(long);

int create_queue(char *path, char id, int flag) {
    key_t key = ftok(path, id);

    if (key == -1) {
        eprint("cannot get key for queue, errno: %s", strerror(errno));
    }

    int q;
    if ((q = msgget(key, flag)) == -1) {
        eprint("cannot create queue, errno: %s", strerror(errno));
    }
    return q;
}

void close_queue(int q) {
    msgctl(q, IPC_RMID, NULL);
}

#define send_message(queue, m, flags) { \
if (msgsnd(queue, &m, message_size, flags) == -1){ \
    eprint("cannot send message, errno: %s", strerror(errno));}}

#define receive_message_NOWAIT(queue, mtype, message) { \
if (msgrcv(queue, &message, message_size, mtype, IPC_NOWAIT) == -1) continue;}

#define receive_message(queue, mtype, message) { \
if (msgrcv(queue, &message, message_size, mtype, 0) == -1) { \
eprint("cannot receive message, errno: %s", strerror(errno));\
}}



#endif //SYSOPY_MY_MESSAGES_H
