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
#include <sys/un.h>
#include <sys/poll.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#define PORT 8081

#define MAX_CLIENTS 4
#define SERVER_ID_LOCAL 0
#define SERVER_ID_NETWORK 1
#define ALL_FDS 2

char* client_names[MAX_CLIENTS];
symbol client_symbols[MAX_CLIENTS];
int clients_games[MAX_CLIENTS];
char boards[MAX_CLIENTS][10];

int clients[MAX_CLIENTS];
int client_fds[MAX_CLIENTS];
bool clients_is_online[MAX_CLIENTS];
struct sockaddr client_addr[MAX_CLIENTS];

struct pollfd fds[ALL_FDS];
#define EMPTY (0)
#define TAKEN (1)
#define NAME_UNKNOWN (2)
int curr_clients = 0;
int last_client = -1;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

bool check_name(char *name){
    for_i(MAX_CLIENTS){
        if (clients[i] == TAKEN) {
//            print("%d", i)
            if (equals(name, client_names[i])) {
                return true;
            }
        }
    }
    return false;
}

int add_client(char* name, int fd, struct sockaddr sockaddr){
    if (curr_clients == MAX_CLIENTS)
        return -1;

    if (check_name(name))
        return -1;

    print("ADDING CLIENT %s", name);

    int i;
//
    for___(MAX_CLIENTS, i){
        if (clients[i] == EMPTY){
//            print("%d", i)
            clients[i] = TAKEN;
            client_addr[i] = sockaddr;
            clients_is_online[i] = true;
            client_names[i] = copy_string(name);
            client_fds[i] = fd;

            sendto(fd, good_name_message, strlen(good_name_message), 0, &sockaddr, sizeof (sockaddr));
            break;
        }
    }


    curr_clients ++;
    if (curr_clients > 1 and curr_clients % 2 == 0) {
        start_game(last_client, i, boards, client_symbols,
                   clients_games, client_fds, client_addr[last_client], client_addr[i]);
    }
    print("CLIENTS %d", curr_clients);

    last_client = i;

    return i;
}

void end_disconnected(int i, bool won){
    game_info info = {.end = true, .you_won = won, .my_turn = true};
    char* message = prepare_message(client_symbols[i], boards[i], &info);
    print("END DISCONNECTED %d", clients[i])
    send(fds[i].fd, message, strlen(message), 0); //MSG_NOSIGNAL

    if (sendto(client_fds[i], message, strlen(message), 0, &client_addr[i], sizeof (client_addr[i])) < 0){
        perror("SEND to 1");
    }
}

void delete_client(int i){
    if (clients[i] == EMPTY){
//        print("GOTCHA %d", i)
        return;
    }

    clients[i] = EMPTY;

    print("DELETING %d", i);

    if (clients[clients_games[i]] != EMPTY){
        print("DELETING FROM %d to %d", i, clients_games[i]);
        end_disconnected(clients_games[i], true);
    }

//    fds[i].fd = -1;
    curr_clients--;

    print("CLIENTS %d", curr_clients);

}



int get_by_addr(struct sockaddr *sockaddr){
    for_i(MAX_CLIENTS){
        if(clients[i] != TAKEN)
            continue;
        if (equals(sockaddr->sa_data, client_addr[i].sa_data) and sockaddr->sa_family == client_addr[i].sa_family) {
//            print("%d", i)
            return i;
        }
    }
    return -1;
}

int init_server_network(int port){
    int opt = 1;
    int socket_network;
    if ((socket_network = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK , 0)) == 0){
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


    fds[SERVER_ID_NETWORK].fd = socket_network;
    fds[SERVER_ID_NETWORK].events = POLLIN;

    return socket_network;
}


int init_server_local(char* path){
    int socket_local;
    if ((socket_local = socket(AF_UNIX, SOCK_DGRAM | SOCK_NONBLOCK, 0)) == 0){
        perror("SOCKET FAILED");
    }

    struct sockaddr_un address;
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, path);
    int opt=1;
    if (setsockopt(socket_local, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("SET SOCKET OPT FAILED");
    }

//    unlink(path);
    if (bind(socket_local, (struct sockaddr *) &address, sizeof (address)) < 0){
        perror("BIND FAILED");
    }


    fds[SERVER_ID_LOCAL].fd = socket_local;
    fds[SERVER_ID_LOCAL].events = POLLIN;

    return socket_local;
}


void *ping(void *arg){
    while (true) {
        pthread_mutex_lock(&mutex);

        for_i(curr_clients){
            if (clients_is_online[i] == false){
                print("%d offline", i);
                end_disconnected(i, false);
                delete_client(i);
            }
        }

        for_i(curr_clients){
            clients_is_online[i] = false;
            sendto(client_fds[i], ping_message, strlen(ping_message), 0,&client_addr[i], sizeof (client_addr[i]));

        }

        pthread_mutex_unlock(&mutex);
        sleep_seconds(5);
    }

    return NULL;
}

//void login_client(char* name, int i){
//    if (check_name(name)) {
//        client_names[i] = copy_string(name);
//        clients[i] = TAKEN;
//
//        char message[] = "good";
//        send(fds[i].fd, message, strlen(message), 0);
//
//        curr_clients ++;
//        if (curr_clients > 1 and curr_clients % 2 == 0) {
//            start_game(last_client, i, boards, client_symbols, clients_games, fds);
//        }
//        print("CLIENTS %d", curr_clients);
//
//        last_client = i;
//    } else {
//        print("NAME %s taken", name);
//        char message[message_size];
//
//        sprintf(message, wrong_name_format, name);
//        send(fds[i].fd, message, strlen(message), 0);
//        delete_client(i);
//    }
//}

//void accept_clients(int fd){
////    int new_sd = accept(fd, NULL, NULL);
////    while (new_sd != -1) {
////        int client_id = add_client(new_sd);
////        print("NEW CLIENT %d", client_id)
////        new_sd = accept(fd, NULL, NULL);
////    }
//}

int main(void) {
    char buffer[128];

    memset(fds, 0, sizeof(fds));

    int timeout = (3.5 * 60 * 1000);
    int rc;
    bool close_server = false;

    char name[] = "server_socket";
    init_server_network(PORT);
    init_server_local(name);

    pthread_t ping_thread;
    pthread_create(&ping_thread, NULL, ping, NULL);


    while (close_server == false) {
        rc = poll(fds, ALL_FDS, timeout);

        if (rc < 0) {
            perror("ERROR IN POLL");
        } else if (rc == 0) {
            print("POLL TIMEOUT", NULL);
        }
//        print_array(clients, MAX_CLIENTS, "%d ");
        for_i(ALL_FDS) {
            if (fds[i].revents == 0) {
                continue;
            }
            if (not(fds[i].revents & POLLIN)) {
                print("ERROR !!!!! REVENTS[i] = %d", fds[i].revents)
            }
            pthread_mutex_lock(&mutex);

            {
                memset(buffer, 0, sizeof(buffer));
                struct sockaddr from_addr;
                socklen_t from_length = sizeof(struct sockaddr);
                rc = recvfrom(fds[i].fd, buffer, sizeof(buffer), 0, &from_addr, &from_length);


                if (rc < 0) {
                    print("RECV FAILED", NULL)
                    perror(":((");
                    continue;
                } else if (rc == 0) {
                    continue;
                }

//                print("message %s", buffer)

                char name[message_size];

                if (sscanf(buffer, login_format, name) == 1) {

                    int j = add_client(name, fds[i].fd, from_addr);

                    if (j >= 0) {
                        print("CLIENT ADDED %d", j)


                    } else{
                        print("CLIENT NOT ADDED %d", j)
                    }



                } else {
                    int client = get_by_addr(&from_addr);
//                    print("%d", client);
                    if (equals(ping_message, buffer)) {
//                        print("GOT PING %d", client);
                        clients_is_online[client] = true;
                    } else if (equals(exit_message, buffer)) {
                        delete_client(client);
                    } else {

                        server_got_position(buffer, client, boards, client_symbols,
                                            clients_games, client_fds, client_addr);
                    }
                }
            }
            pthread_mutex_unlock(&mutex);
        }

    }

    pthread_join(ping_thread, NULL);
    return 0;
}



#pragma clang diagnostic pop