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

/*
 * Napisz program serwera w ktorym stworzysz socket domeny unixowej,
 * typu datagramowego następnie napisz program klienta który polaczy
 * się z serwerem przesle komunikat HELLO to proces:   i tutaj pid klienta

Zadanie serwera to wypisac na ekran otrzymany pid.

Na poczatek przyjmij ze masz jednego klienta, od którego otrzymasz pojedyncza wiadomość
 */

#define PORT 8080

int main(void) {
    int sock = 0;
    struct sockaddr_in server_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("SOCKET FAILED");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr)<=0){
        perror("INET PTON FAILED");
    }

    if (connect(sock, (struct sockaddr *) &server_addr, sizeof (server_addr)) <0){
        perror("CONNECT FAILED");
    }



    char hello[] = "HELLO!";
    char* pid = int_to_string(getpid());

    char to_send[strlen(hello) + strlen(pid) + 5];

    sprintf(to_send, "%s %s", hello, pid);

    send(sock, to_send, strlen(to_send),0);

    return 0;
}


