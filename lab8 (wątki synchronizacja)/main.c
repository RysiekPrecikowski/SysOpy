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

/*
 * Napisz program w którym uruchomisz n watkow (parametr programu)
 * realizujacych te sama funkcje. W ramach swojego dzialania watki
 * maja za zadanie inkrementowac wartosc zmiennej globalnej i wypisywac
 * jej wartosc na konsole wraz z informacja identyfikujaca watek
 * dokonujacy wypisania. Dostep do zmiennej zabezpiecz mutexem.
 */

/*
 * Zmodyfikuj poprzednie w taki sposób ze zmienisz atrybuty mutexu odpowiednio na

PTHREAD_MUTEX_NORMAL

PTHREAD_MUTEX_ERRORCHECK

PTHREAD_MUTEX_RECURSIVE

PTHREAD_MUTEX_DEFAULT

I sprawdzisz dzialanie programu w kazdym z przypadkow.
 Sprobuj doprowadzic do potweirdzenia „wlaczenia” kazdej z tych opcji.
 */

int v;
pthread_mutex_t v_mutex;


void* thread_print(void *arg){
    int i = *(int *) arg;
    int j=0;
    while (1) {
        j++;

        pthread_mutex_lock(&v_mutex);
        v++;
//        printf("watek nr: %-4d j: %-7d v: %-8d id: %lu\n", i, j, v, pthread_self());

        pthread_mutex_unlock(&v_mutex);
        if (j >= 10000)
            break;
    }


    pthread_exit(NULL);
}

int main(void){
    int n = 10;
    v=0;
    pthread_mutexattr_t mutexattr;

    if (pthread_mutexattr_init(&mutexattr) == -1){
        perror("creating mutex attr");
    }

    if (pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_ERRORCHECK) == -1){
        perror("setting type");
    }

    int type;

    if (pthread_mutexattr_gettype(&mutexattr, &type) == -1){
        perror("getting type");
    }

    printf("attr type %d\n", type);

    if (pthread_mutex_init(&v_mutex, &mutexattr) == -1){
        perror("creating mutex");
    }


    ///test
    pthread_mutex_lock(&v_mutex);
    int code;
    if ((code = pthread_mutex_lock(&v_mutex)) != 0)
        fprintf(stderr, "2nd lock: %s", strerror(code));
    else
        printf("2x locked\n");

    pthread_mutex_unlock(&v_mutex);
    pthread_mutex_unlock(&v_mutex);

    pthread_t threads[n];
    int starting[n];
    int i;
    for(i = 0; i < n ; i++){
        starting[i] = i;
        pthread_create(&threads[i], NULL, &thread_print, &starting[i]);
    }

    printf("\n\n");
    for(i = 0 ; i < n ; i++){
        void* r_value;
        pthread_join(threads[i], &r_value);
        printf("watek nr: %-4d zakonczony\n", i);
        free(r_value);
    }


    printf("\nV: %d\n", v);

    pthread_mutex_destroy(&v_mutex);
    pthread_mutexattr_destroy(&mutexattr);
    return 0;
}