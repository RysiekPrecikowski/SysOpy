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

void sig_handler(int sig){
    exit(-1);
}

void bye(){
    printf("BYE BYE!!!\n");
    send(socket_global, exit_message, strlen(exit_message), MSG_DONTWAIT );
}

#define LOCAL 0
#define NETWORK 1

struct sockaddr_un address;
int init_connection(int type, char* arg){
    int sock;
    if (type == LOCAL){
        if ((sock = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
            perror("SOCKET FAILED");
        }

        address.sun_family = AF_UNIX;

        strcpy(address.sun_path, arg);
        if (connect(sock, (struct sockaddr *) &address, sizeof(address)) < 0) {
            perror("CONNECT FAILED");
        }

    } else if (type == NETWORK){
        struct sockaddr_in server_addr;

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("SOCKET FAILED");
        }
        int port = string_to_int(arg);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
            perror("INET PTON FAILED");
        }

        if (connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
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

    int connection_type = LOCAL;

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

    struct pollfd fds[2];

    fds[IN_FD].fd = fileno(stdin);
    fds[IN_FD].events = POLLIN;

    fds[SERV_FD].fd = sock;
    fds[SERV_FD].events = POLLIN;

    char position[10];

    int rc;

    char *name = argv[1];
    char login_message[message_size];

    char buffer[100];

    sprintf(login_message, login_format, name);

    sendto(sock, login_message, strlen(login_message), 0, &address, sizeof (address));

    rc = recv(sock, buffer, sizeof(buffer), 0);

    if (rc < 0) {
        print("RECV FAILED", NULL)
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


    return 0;

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
                rc = recv(sock, buffer, sizeof(buffer), 0);

                if (rc < 0) {
                    print("RECV FAILED", NULL)
                } else if (rc == 0) {
                    print("CLOSED CONNECTION", NULL);
                    exit(1);
                } else {
                    if (equals(ping_message, buffer)){
                        send(sock, buffer, strlen(buffer), 0);
                    } else {
                        read_message(buffer, info);
                    }
                }
            } else if (i == IN_FD){
                if (info->my_turn) {
                    memset(position, 0, sizeof(position));

                    fscanf(stdin, "%s", position);

                    send(sock, position, strlen(position), 0);
                }
            }
        }
    }


    return 0;
}


