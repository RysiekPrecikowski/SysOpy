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


/*
 * Napisz program serwera w ktorym stworzysz socket domeny unixowej,
 * typu datagramowego następnie napisz program klienta który polaczy
 * się z serwerem przesle komunikat HELLO to proces:   i tutaj pid klienta

Zadanie serwera to wypisac na ekran otrzymany pid.

Na poczatek przyjmij ze masz jednego klienta, od którego otrzymasz pojedyncza wiadomość
 */

#define PORT 8080

int main(void){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt=1;
    int addrlen = sizeof (address);
    char buffer[128] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("SOCKET FAILED");
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("SET SOCKET OPT FAILED");
    }

    address.sin_family=AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);


    if (bind(server_fd, (struct sockaddr *) &address, sizeof (address)) < 0){
        perror("BIND FAILED");
    }

    if (listen(server_fd, 1) < 0)
    {
        perror("LISTEN FAILED");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen))<0){
        perror("ACCEPT FAILED");
    }

    read(new_socket, buffer, sizeof (buffer));
    print("MESSAGE RECEIVED %s", buffer);

    return 0;
}


