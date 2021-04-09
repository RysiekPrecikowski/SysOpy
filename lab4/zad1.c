#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

/*
 * Napisz program który wysle sygnal SIGUSR1 do samego siebie (raise).
 * Funkcją signal obsluz sygnal w taki sposob ze wydrukujesz na konsoli
 * komunikat typu „ja cie ale sygnal”
 */

void handleSIGUSR1(int sig){
    printf("ja cie ale sygnal \n");
}

int main(void){
    signal(SIGUSR1, handleSIGUSR1);

    raise(SIGUSR1);


    return 0;
}