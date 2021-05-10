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
#include "zad1/utils.h"

/*
 * Napisz program w którym storzysz n watkow (parametr programu)
 * w którym każdy w nieskonczonej petli wypisuje na konsole napis typu

    ” Hello tu watek n. Iteracja m”. Dodatkowo po stworzeniu wszystkich
    watkow program głowny także (w nieskonczonej petli) wypisuje na konsole
    napis typu „Hello tu program głowny Iteracja m”
 */



void* thread_print(void *arg){
    int i = *(int *) arg;
    int j= 0;
    while (1) {
        printf("watek nr: %-4d iteracja %-7d id: %lu\n", i, j++, pthread_self());
        sleep_seconds(0.1);

        if (j > 10)
            break;
    }

    int* res = malloc(sizeof (int));
    *res = i;
    return res;
}

int main(void){
    int n = 4;

    pthread_t threads[n];
    int starting[n];
    int i;
    for(i = 0; i < n ; i++){
        starting[i] = i;
        pthread_create(&threads[i], NULL, &thread_print, &starting[i]);
    }

    i=0;
    while (1){
        printf("Program główny iteracja %d\n", i++);
        sleep_seconds(0.1);
        if (i > 10)
            break;
    }
    printf("\n\n");
    for(i = 0 ; i < n ; i++){
        void* r_value;
        pthread_join(threads[i], &r_value);
        printf("watek nr: %-4d zakonczony %d\n", i, *(int*) r_value);
        free(r_value);
    }

    return 0;
}