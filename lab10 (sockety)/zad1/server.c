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
#include "tic_tac_toe.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <sys/poll.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#define PORT 8081

#define MAX_CLIENTS 4
#define SERVER_ID_LOCAL MAX_CLIENTS
#define SERVER_ID_NETWORK (MAX_CLIENTS + 1)
#define ALL_FDS (SERVER_ID_NETWORK + 1)

char* client_names[MAX_CLIENTS];
symbol client_symbols[MAX_CLIENTS];
int clients_games[MAX_CLIENTS];
char boards[MAX_CLIENTS][10];

int clients[ALL_FDS];

struct pollfd fds[ALL_FDS];
#define EMPTY (0)
#define TAKEN (1)
#define NAME_UNKNOWN (2)
int curr_clients = 0;
int last_client = -1;

int add_client(int fd){
    if (curr_clients == MAX_CLIENTS)
        return -1;

    int i;
    print("adding client", NULL)
    for___(MAX_CLIENTS, i){
        if (clients[i] == EMPTY){
            clients[i] = NAME_UNKNOWN;
            fds[i].fd = fd;
            fds[i].events = POLLIN;
            break;
        }
    }
    return i;
}

void end_disconnected(int i){
    game_info info = {.end = true, .you_won = true};
    char* message = prepare_message(client_symbols[i], boards[i], &info);
    send(fds[i].fd, message, strlen(message), 0);
}

void delete_client(int i){
    clients[i] = EMPTY;

    fds[i].fd = -1;

    if (clients[clients_games[i]] != EMPTY){
        end_disconnected(clients[clients_games[i]]);

        if (curr_clients % 2 == 1 and curr_clients > 1){
            start_game(last_client, clients_games[i], boards, client_symbols, clients_games, fds);
        }
        last_client = clients_games[i];
        print("LAST %d", last_client);
    }

    curr_clients--;
}

bool check_name(char *name){
    for_i(MAX_CLIENTS){
        if (clients[i] != TAKEN)
            continue;
        if (equals(name, client_names[i])){
            return false;
        }
    }
    return true;
}


int init_server_network(int port){
    int opt = 1;
    int socket_network;
    if ((socket_network = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK , 0)) == 0){
        perror("SOCKET FAILED");
    }
    if (setsockopt(socket_network, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("SET SOCKET OPT FAILED");
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(socket_network, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("BIND FAILED");
    }

    if (listen(socket_network, 10) < 0) {
        perror("LISTEN FAILED");
        exit(EXIT_FAILURE);
    }
    fds[SERVER_ID_NETWORK].fd = socket_network;
    fds[SERVER_ID_NETWORK].events = POLLIN;
    clients[SERVER_ID_NETWORK] = TAKEN;

    return socket_network;
}

int main(void) {
    char buffer[128] = {0};

    memset(fds, 0, sizeof(fds));

    int timeout = (3.5 * 60 * 1000);
    int rc;
    bool close_server = false;

    init_server_network(PORT);

    int new_sd;
    while (close_server == false) {
        print_array(clients, ALL_FDS, "%d ");

        rc = poll(fds, ALL_FDS, timeout);

        if (rc < 0) {
            perror("ERROR IN POLL");
        } else if (rc == 0) {
            print("POLL TIMEOUT", NULL);
        }

        for_i(ALL_FDS) {
            if (fds[i].revents == 0 or clients[i] == EMPTY) { //TODO CO TO JEST TO ZERO?
                // Loop through to find the descriptors
                // that returned POLLIN and determine whether
                // it's the listening or the active connection.
//                print("revents 0 or empty %d", i);
                continue;
            }
            if (not (fds[i].revents & POLLIN)) {
                print("ERROR !!!!! REVENTS[i] = %d", fds[i].revents)
            }

            if (i == SERVER_ID_NETWORK or i == SERVER_ID_LOCAL) {
                print("SERVER LISTENER HERE !!!!", NULL);

                new_sd = accept(fds[i].fd, NULL, NULL);
                while (new_sd != -1) {
                    print("NEW SD %d", new_sd);
                    print("NEW CLIENT :O", NULL)
                    int client_id = add_client(new_sd);

                    new_sd = accept(fds[i].fd, NULL, NULL);
                }
            } else {
                memset(buffer, 0, sizeof (buffer));
                rc = recv(fds[i].fd, buffer, sizeof (buffer), 0);

                if (rc < 0){
                    print("RECV FAILED", NULL)
                } else if (rc == 0) {
                    // Connection is closed;
                    print("CLOSED CONNECTION %d", i);
                    delete_client(i);
                } else {
                    print("GOT MESSAGE :OOOO %s", buffer);

                    if (clients[i] == NAME_UNKNOWN){
                        char name[message_size];
                        sscanf(buffer, login_format, name);

                        if (check_name(name)) {
                            client_names[i] = copy_string(name);
                            clients[i] = TAKEN;

                            char message[] = "good";

                            send(fds[i].fd, message, strlen(message), 0);

                            curr_clients ++;
                            if (curr_clients > 1 and curr_clients % 2 == 0) {
                                start_game(last_client, i,
                                           boards,
                                           client_symbols,
                                           clients_games,
                                           fds);

                            }
                            print("CLIENTS %d", curr_clients);


                            last_client = i;
                        } else {
                            print("NAME %s taken", name);

                            char message[message_size];

                            sprintf(message, wrong_name_format, name);
                            send(fds[i].fd, message, strlen(message), 0);

                            delete_client(i);
                        }
                    } else {
                        server_got_position(buffer, i,
                                            boards,
                                            client_symbols,
                                            clients_games,
                                            fds);
                    }
                }

            }
        }

    }

    return 0;
}



#pragma clang diagnostic pop