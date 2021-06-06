#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include <pthread.h>
#include "utils.h"


#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>

#include "tic_tac_toe.h"

#define IN_FD 0
#define SERV_FD 1

#define PORT 8081

int socket_global;
struct pollfd fds[2];

#define LOCAL 0
#define NETWORK 1

struct sockaddr_un address;
bool is_local = false;
int sock_in_local;

int server_socket;
int binded_socket;
struct sockaddr_un local_sockaddr;
struct sockaddr_in server_addr;
void sig_handler(int sig){
    exit(-1);
}

void bye(){
    printf("BYE BYE!!!\n");
//    sendto(fds[SERV_FD].fd, exit_message, strlen(exit_message), 0 , (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr_un));

    if (is_local)
        sendto(fds[SERV_FD].fd, exit_message, strlen(exit_message), 0, (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr_un));
    else {
        if (sendto(fds[SERV_FD].fd, exit_message, strlen(exit_message), 0, (struct sockaddr *) &server_addr,
                   sizeof(struct sockaddr_in)) < 0)
            perror("SEND");
    }

    unlink(int_to_string(getpid()));
}





int init_connection(int type, char* arg){
    int sock;
    if (type == LOCAL){
        server_socket = socket(AF_UNIX, SOCK_DGRAM, 0);

        memset(&local_sockaddr, 0, sizeof(struct sockaddr_un));
        local_sockaddr.sun_family = AF_UNIX;
        print("%s", arg);
        strcpy(local_sockaddr.sun_path, arg);

        if (connect(server_socket, (struct sockaddr *)&local_sockaddr,
                sizeof(struct sockaddr_un)) < 0)
            perror("CONNECT");

        binded_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
        struct sockaddr_un binded_sockaddr;
        memset(&binded_sockaddr, 0, sizeof(struct sockaddr_un));
        binded_sockaddr.sun_family = AF_UNIX;

        char tmp_path[20];
        sprintf(tmp_path, "%d", getpid());

        sprintf(binded_sockaddr.sun_path, "%s", tmp_path);
        unlink(tmp_path);
        bind(binded_socket, (struct sockaddr *)&binded_sockaddr,
             sizeof(struct sockaddr_un));

        is_local = true;
    } else if (type == NETWORK){
        if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("SOCKET FAILED");
        }
        int port = string_to_int(arg);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
            perror("INET PTON FAILED");
        }

        if (connect(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
            perror("CONNECT FAILED");
        }
    }

    return sock;
}

int main(int argc, char* argv[]) {
    atexit(bye);
    signal(SIGINT, sig_handler);
    char s[] ="server_socket";

    int sock;

    int connection_type = NETWORK;

    if (equals(argv[2], "NETWORK"))
        connection_type = NETWORK;
    if(equals(argv[2], "LOCAL"))
        connection_type = LOCAL;



    switch (connection_type) {
        case NETWORK:
            sock = init_connection(NETWORK, "8081");
            break;
        case LOCAL:
            sock = init_connection(LOCAL, s);
            break;
    }


    socket_global = sock;

    game_info *info = malloc(sizeof(game_info));

    int timeout = (2.5 * 60 * 1000);



    fds[IN_FD].fd = fileno(stdin);
    fds[IN_FD].events = POLLIN;

    if (is_local)
        fds[SERV_FD].fd = binded_socket;
    else
        fds[SERV_FD].fd = server_socket;

    fds[SERV_FD].events = POLLIN;

    char position[10];

    int rc;

    char *name = argv[1];
    char login_message[message_size];

    char buffer[100];

    sprintf(login_message, login_format, name);

    if (is_local)
        sendto(fds[SERV_FD].fd, login_message, strlen(login_message), 0, (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr_un));
    else {
        if (sendto(fds[SERV_FD].fd, login_message, strlen(login_message), 0, (struct sockaddr *) &server_addr,
                   sizeof(struct sockaddr_in)) < 0)
            perror("SEND");
    }


    rc = recv(fds[SERV_FD].fd, buffer, sizeof(buffer), 0);
    if (rc < 0) {
        print("RECV FAILED", NULL)
        perror(":((");
    } else if (rc == 0) {
        print("CLOSED CONNECTION", NULL);
        return -1;
    }


    if (sscanf(buffer, wrong_name_format, login_message) == 1) {
        perror("NAME WRONG!");
        return -1;
    } else {
        print("LOGGED IN", NULL);
    }



    while (true) {
        sleep(1);
        rc = poll(fds, 2, timeout);

        if (rc < 0) {
            perror("ERROR IN POLL");
        }

        for_i(2) {
            if (fds[i].revents == 0)
                continue;

            if (not(fds[i].revents & POLLIN)) {
                print("ERROR !!!!! REVENTS[i] = %d", fds[i].revents)
            }

            if (i == SERV_FD) {
                memset(buffer, 0, sizeof (buffer));
                rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);

                if (rc < 0) {
                    print("RECV FAILED", NULL)
                } else if (rc == 0) {
                    print("CLOSED CONNECTION", NULL);
                    exit(1);
                } else {
                    if (equals(ping_message, buffer)){
//                        printf("got ping\n");

                        if (is_local)
                            sendto(fds[SERV_FD].fd, buffer, strlen(buffer), 0, (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr_un));
                        else {
                            if (sendto(fds[SERV_FD].fd, buffer, strlen(buffer), 0, (struct sockaddr *) &server_addr,
                                       sizeof(struct sockaddr_in)) < 0)
                                perror("SEND");
                        }

//                        sendto(fds[SERV_FD].fd, buffer, strlen(buffer), 0, (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr_un));
                    } else {
                        read_message(buffer, info);
                    }
                }
            } else if (i == IN_FD){
                if (info->my_turn) {
                    memset(position, 0, sizeof(position));

                    fscanf(stdin, "%s", position);

//                    char message[100];

//                    sprintf(message, move_message_format, name, position);

                    if (is_local)
                        sendto(fds[SERV_FD].fd, position, strlen(position), 0, (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr_un));
                    else {
                        if (sendto(fds[SERV_FD].fd, position, strlen(position), 0, (struct sockaddr *) &server_addr,
                                   sizeof(struct sockaddr_in)) < 0)
                            perror("SEND");
                    }


//                    sendto(fds[SERV_FD].fd, position, strlen(position),
//                           0, (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr_un));

                }
            }
        }
    }


    return 0;
}


