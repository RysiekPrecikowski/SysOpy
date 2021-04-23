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

#include <time.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<signal.h>
#include<string.h>
#include<mqueue.h>
#include<errno.h>


#include "utils.h"




enum TYPES {
    STOP = 2, INIT, DISCONNECT, CONNECT, LIST, CHAT, ALL_TYPES
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
#define CLIENT_ID (getpid() % MAX_CLIENTS)
#define HOME getenv("HOME")
#define SERVER_PATH "/server_posix"
#define CLIENT_PATH "/client_posix"

#define MAX_MESSAGE_NUM 10
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

#define message_size (sizeof (message) + sizeof(mqd_t) + sizeof (int) + 96)

int create_queue(char* path, int flag, mode_t mode) {
    struct mq_attr attr;
    attr.mq_curmsgs = 0;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGE_NUM;
    attr.mq_msgsize = message_size;

    return mq_open(path, flag, mode, &attr);
}

int create_server_queue(){
    return create_queue((char*)SERVER_PATH, O_CREAT | O_RDWR,0666); //TODO rdonly
}

int open_server_queue(){
     //TODO rdonly
    return mq_open((char*) SERVER_PATH, O_RDWR);
}

char* get_client_path(char* id){
    char *path = calloc(strlen(CLIENT_PATH) + INTLEN + 1, sizeof (char));
    sprintf(path, "%s%s", CLIENT_PATH, id);
    return path;
}

int create_client_queue(char* id){
    char *path = calloc(strlen(CLIENT_PATH) + INTLEN + 1, sizeof (char));
    sprintf(path, "%s%s", CLIENT_PATH, id);
    return create_queue(path, O_CREAT | O_RDWR, 0666);
}



void close_queue(int q) {
    mq_close(q);
//    mq_unlink(SERVER_PATH);
}

#define send_message(queue, m) { \
if (mq_send(queue, (char*) &m, message_size, m.mtype) == -1){ \
    eprint("cannot send message, errno: %s", strerror(errno));}}

//TODO
#define receive_message_NOWAIT(queue, mtype, message) { \
struct timespec tm;\
clock_gettime(CLOCK_REALTIME, &tm);\
tm.tv_sec += 1;\
if (mq_timedreceive(queue,(char*) &message, message_size, &mtype, &tm) == -1) continue;}

#define receive_message(queue, mtype, message) { \
if (mq_receive(queue,(char*) &message, message_size, &mtype) == -1) { \
eprint("cannot receive message, errno: %s", strerror(errno));\
}}



#endif //SYSOPY_MY_MESSAGES_H
