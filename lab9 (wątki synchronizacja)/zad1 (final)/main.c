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
#include "names.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"


#define ELF_WORKING_TIME (rand() % 3 + 2)
#define REINDEER_HOLIDAY_TIME (rand() % 6 + 5)
#define GIFTS_DELIVERY_TIME (rand() % 3 + 2)
#define PROBLEM_SOLVING_TIME (rand() % 2 + 1)

#define ELVES_WAITING 3

typedef struct Reindeer{
    int name;
} Reindeer;

typedef struct Elf{
    int name;
} Elf;

#define lock_mutex(mutex_t) {pthread_mutex_lock(&mutex_t);}

#define unlock_mutex(mutex_t) {pthread_mutex_unlock(&mutex_t);}


/// Santa mutexes
pthread_mutex_t santa_awake_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_awake_cond = PTHREAD_COND_INITIALIZER;
bool santa_awake = false;

bool elves_at_santa = false;
pthread_mutex_t elves_at_santa_mutex = PTHREAD_MUTEX_INITIALIZER;

bool reindeer_at_santa = false;
pthread_mutex_t reindeer_at_santa_mutex = PTHREAD_MUTEX_INITIALIZER;


/// Elves mutexes
int n_elves_waiting = 0;
pthread_mutex_t n_elves_waiting_mutex = PTHREAD_MUTEX_INITIALIZER;

bool elves_are_waiting_for_santa = false;
pthread_mutex_t elves_are_waiting_for_santa_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t elves_are_waiting_for_santa_cond = PTHREAD_COND_INITIALIZER;


/// Reindeer mutexes
int n_waiting_reindeer = 0;
pthread_mutex_t n_waiting_reindeer_mutex = PTHREAD_MUTEX_INITIALIZER;

bool reindeer_are_waiting = false;
pthread_mutex_t reindeer_are_waiting_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t reindeer_are_waiting_cond = PTHREAD_COND_INITIALIZER;


void* elf_thread(void* arg){
    Elf *elf = (Elf *) arg;
    srand(pthread_self() * time(NULL));

    print("hi, im elf      %d %s", elf->name, elf_name(elf->name));

    while (true){
        sleep_seconds(ELF_WORKING_TIME);

        lock_mutex(elves_are_waiting_for_santa_mutex);
        while (elves_are_waiting_for_santa == true){
            pthread_cond_wait(&elves_are_waiting_for_santa_cond, &elves_are_waiting_for_santa_mutex);
        }

        lock_mutex(n_elves_waiting_mutex);  /// można sie pozbyc jednego mutexa, ale wtedy to jest mniej czytelne, wiec zostawie na 2
        n_elves_waiting ++;
        if (n_elves_waiting == 3)
            elves_are_waiting_for_santa = true;

        unlock_mutex(elves_are_waiting_for_santa_mutex);

        print("ELF %d %-20s, waiting %d", elf->name, elf_name(elf->name), n_elves_waiting);

        if (n_elves_waiting == 3){
            print("ELF %d %-20s--> Santa, wake up!", elf->name, elf_name(elf->name));

            lock_mutex(santa_awake_mutex);

            elves_at_santa = true;
            santa_awake = true;
            pthread_cond_broadcast(&santa_awake_cond);

            unlock_mutex(santa_awake_mutex);
        }

        unlock_mutex(n_elves_waiting_mutex);
    }
    pthread_exit(NULL);
}



void* reindeer_thread(void* arg){
    Reindeer *reindeer = (Reindeer *) arg;
    srand(pthread_self() * time(NULL));


    print("hi, im reindeer %d %s", reindeer->name, reindeer_name(reindeer->name));

    while (true){
        lock_mutex(reindeer_are_waiting_mutex);
        while (reindeer_are_waiting == true){
            pthread_cond_wait(&reindeer_are_waiting_cond, &reindeer_are_waiting_mutex);
        }
        unlock_mutex(reindeer_are_waiting_mutex);

        sleep_seconds(REINDEER_HOLIDAY_TIME);

        lock_mutex(n_waiting_reindeer_mutex);
        n_waiting_reindeer += 1;

        reindeer_are_waiting = true;

        print("reindeer %d %-15s, waiting: %d", reindeer->name, reindeer_name(reindeer->name), n_waiting_reindeer);
        if (n_waiting_reindeer == ALL_REINDEER){
            print("reindeer %d %-15s--> Santa, wake up!", reindeer->name, reindeer_name(reindeer->name));

            lock_mutex(santa_awake_mutex);

            reindeer_at_santa = true;
            santa_awake = true;
            pthread_cond_broadcast(&santa_awake_cond);
            unlock_mutex(santa_awake_mutex);
        }

        unlock_mutex(n_waiting_reindeer_mutex);
    }
    pthread_exit(NULL);
}



void* santa_thread(void* arg){
    srand(pthread_self() * time(NULL));

    print("hi, im SANTA", NULL)

    while (true){
        lock_mutex(santa_awake_mutex);
        while (santa_awake == false){
            pthread_cond_wait(&santa_awake_cond, &santa_awake_mutex);
        }
        printf("\n");
        print("SANTA AWAKENING", NULL);
        unlock_mutex(santa_awake_mutex);

        lock_mutex(reindeer_at_santa_mutex);
        if (reindeer_at_santa) {
            reindeer_at_santa = false;
            print("SANTA DELIVERING GIFTS", NULL);
            sleep_seconds(GIFTS_DELIVERY_TIME);

            n_waiting_reindeer = 0;
            lock_mutex(reindeer_are_waiting_mutex);
            reindeer_are_waiting = false;
            pthread_cond_broadcast(&reindeer_are_waiting_cond);
            unlock_mutex(reindeer_are_waiting_mutex);
        }
        unlock_mutex(reindeer_at_santa_mutex);

        lock_mutex(elves_at_santa_mutex);

        if(elves_at_santa){
            elves_at_santa = false;
            print("SANTA SOLVING PROBLEMS", NULL);
            sleep_seconds(PROBLEM_SOLVING_TIME);


            lock_mutex(n_elves_waiting_mutex);
            n_elves_waiting = 0;
            unlock_mutex(n_elves_waiting_mutex);


            lock_mutex(elves_are_waiting_for_santa_mutex);
            elves_are_waiting_for_santa = false;
            pthread_cond_broadcast(&elves_are_waiting_for_santa_cond);
            unlock_mutex(elves_are_waiting_for_santa_mutex);
        }
        unlock_mutex(elves_at_santa_mutex);


        lock_mutex(santa_awake_mutex);
        santa_awake = false;
        print("SANTA FALLING ASLEEP", NULL);
        printf("\n");
        unlock_mutex(santa_awake_mutex);
    }
    pthread_exit(NULL);
}

void destroy_mutexes();

int main(void){
    atexit(destroy_mutexes);
    Elf *elves_info = calloc(ALL_ELVES, sizeof (Elf));
    Reindeer *reindeer_info = calloc(ALL_REINDEER, sizeof (Reindeer));

    pthread_t elves_threads[ALL_ELVES];
    pthread_t reindeer_threads[ALL_REINDEER];
    pthread_t santa_thread_t;

    int elf, reindeer;

    pthread_create(&santa_thread_t, NULL, &santa_thread, NULL);

    for___(ALL_ELVES, elf){
        elves_info[elf].name = elf;
        pthread_create(&elves_threads[elf], NULL, &elf_thread, &elves_info[elf]);
    }

    for___(ALL_REINDEER, reindeer){
        reindeer_info[reindeer].name = reindeer;
        pthread_create(&reindeer_threads[reindeer], NULL, &reindeer_thread, &reindeer_info[reindeer]);
    }




    for___(ALL_ELVES, elf){
        pthread_join(elves_threads[elf], NULL);
    }

    for___(ALL_REINDEER, reindeer){
        pthread_join(reindeer_threads[reindeer], NULL);
    }

    pthread_join(santa_thread_t, NULL);

    free(reindeer_info);
    free(elves_info);


    return 0;
}


void destroy_mutexes(){
    pthread_mutex_destroy(&santa_awake_mutex);
    pthread_cond_destroy(&santa_awake_cond);
    pthread_mutex_destroy(&elves_at_santa_mutex);
    pthread_mutex_destroy(&reindeer_at_santa_mutex);


    pthread_mutex_destroy(&n_elves_waiting_mutex);
    pthread_mutex_destroy(&elves_are_waiting_for_santa_mutex);
    pthread_cond_destroy(&elves_are_waiting_for_santa_cond);


    pthread_mutex_destroy(&n_waiting_reindeer_mutex);
    pthread_mutex_destroy(&reindeer_are_waiting_mutex);
    pthread_cond_destroy(&reindeer_are_waiting_cond);
}

#pragma clang diagnostic pop