#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
/*
 * Napisz program który tworzy proces potomny a nastepnie w (nieskonczonej) petli wysyla do
 * swojego potomka sygnaly SIGUSR1. Potomek powinien zliczac otrzymane od przodka
 * sygnaly SIGUSER1 (i wyswietlac na biezaco stan licznika)
 */

void handleSIGUSR1(int sig){
    printf("ja cie ale sygnal \n");
}

int main(void){


    pid_t parent_pid = getpid();
    pid_t child_pid = fork();

    if (child_pid == 0){
        printf("dzieciak\n");
        printf("xd\n");
        signal(SIGUSR1, handleSIGUSR1);
        while (1);
    }


    if (getpid() == parent_pid){
        kill(child_pid, SIGUSR1);
        sleep(1);
    }




    return 0;
}