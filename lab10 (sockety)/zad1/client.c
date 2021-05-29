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
#include <arpa/inet.h>

#include "tic_tac_toe.h"

#define IN_FD 0
#define SERV_FD 1

#define PORT 8081

void sig_handler(int sig){
    exit(-1);
}

void bye(){
    printf("BYE BYE!!!\n");
}

int main(int argc, char* argv[]) {
    atexit(bye);
    signal(SIGINT, sig_handler);
    int sock = 0;
    struct sockaddr_in server_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("SOCKET FAILED");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("INET PTON FAILED");
    }

    if (connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("CONNECT FAILED");
    }

    int rc;

    char *name = argv[1];
    char login_message[message_size];

    char buffer[100];

    sprintf(login_message, login_format, name);

    send(sock, login_message, strlen(login_message), 0);

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

    game_info *info = malloc(sizeof(game_info));

    int timeout = (2.5 * 60 * 1000);


    struct pollfd fds[2];

    fds[IN_FD].fd = fileno(stdin);
    fds[IN_FD].events = POLLIN;

    fds[SERV_FD].fd = sock;
    fds[SERV_FD].events = POLLIN;

    bool read = false;
    char position[10];
    while (true) {

        rc = poll(fds, 2, timeout);

        if (rc < 0) {
            perror("ERROR IN POLL");
        } else if (rc == 0) {
//            print("POLL TIMEOUT", "");
        }

        for_i(2) {
            if (fds[i].revents == 0)
                continue;


            if (not(fds[i].revents & POLLIN)) {
                print("ERROR !!!!! REVENTS[i] = %d", fds[i].revents)
            }

            if (i == SERV_FD) {
                rc = recv(sock, buffer, sizeof(buffer), 0);


                if (rc < 0) {
                    print("RECV FAILED", NULL)
                } else if (rc == 0) {
                    print("CLOSED CONNECTION", NULL);
                    exit(1);
                } else {
                    read_message(buffer, info);
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


