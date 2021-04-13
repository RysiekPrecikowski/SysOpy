#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

/*
 Napisz program w którym stworzysz proces potomny a nastepnie przeslesz od rodzica do potomka komunikat
 typu hello world z wykorzystaniem potoku nienazwanego

Co się stanie w dziecku jeśli rodzic nic tam nie wysle -----> dziecko bedzie czekac (read)

Co się stanie w dziecku jeśli deskryptor do zapisu zostanie w rodzicu zamkniety ----> pipe bedzie pusty, wiec nic nie odczyta

Co się stanie w rodzicu jeśli deskryptor do odczytu zostanie w dziecku zamknięty ----> w rodzicu nic się nie stanie, dziecko nic nie odczyta
        A powinien jakis error xdd
 */

int main(void){
    int fd[2]; // fd[0] -> odczyt, fd[1] -> zapis
    pipe(fd);

    char hello[] = "Hello World";
    char received_hello[strlen(hello) + 1];


    pid_t pid = fork();

    if(pid == 0){
        close(fd[1]);
        close(fd[0]);

        read(fd[0], received_hello, sizeof (received_hello));

        printf("received text: %s", received_hello);

    } else {
        close(fd[0]);
//        close(fd[1]);

        write(fd[1], hello, strlen(hello) + 1);
        wait(NULL);
    }


    return 0;
}