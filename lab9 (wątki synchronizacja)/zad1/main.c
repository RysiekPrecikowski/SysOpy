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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
enum Reindeer_names{Dasher, Dancer, Prancer, Vixen, Comet, Cupid, Donner, Blitzen, Rudolph, ALL_REINDEER};
enum Elves_names {Bushy_Evergreen, Shinny_Upatree, Wunorse_Openslae, Pepper_Minstix, Sugarplum_Mary,
                    Alabaster_Snowball, Pixie, Candy, Garland, Jingle, ALL_ELVES};

char* reindeer_name(int reindeer){
    switch (reindeer) {
        case Dasher:
            return (char*) "Dasher";
        case Dancer:
            return (char*) "Dancer";
        case Prancer:
            return (char*) "Prancer";
        case Vixen:
            return (char*) "Vixen";
        case Comet:
            return (char*) "Comet";
        case Cupid:
            return (char*) "Cupid";
        case Donner:
            return (char*) "Donner";
        case Blitzen:
            return (char*) "Blitzen";
        case Rudolph:
            return (char*) "Rudolph";
        default:
            return (char*) dunno;
    }
}

char* elf_name(int elf){
    switch (elf) {
        case Bushy_Evergreen:
            return (char*) "Bushy Evergreen";
        case Shinny_Upatree:
            return (char*) "Shinny Upatree";
        case Wunorse_Openslae:
            return (char*) "Wunorse Openslae";
        case Pepper_Minstix:
            return (char*) "Pepper Minstix";
        case Sugarplum_Mary:
            return (char*) "Sugarplum Mary";
        case Alabaster_Snowball:
            return (char*) "Alabaster Snowball";
        case Pixie:
            return (char*) "Pixie";
        case Candy:
            return (char*) "Candy";
        case Garland:
            return (char*) "Garland";
        case Jingle:
            return (char*) "Jingle";
        default:
            return (char*) dunno;
    }
}

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

#define lock_mutex(mutex_t) { \
pthread_mutex_lock(&mutex_t);}

#define unlock_mutex(mutex_t){ \
pthread_mutex_unlock(&mutex_t);}


/// Santa mutexes
pthread_mutex_t santa_awake_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_awake_cond = PTHREAD_COND_INITIALIZER;
bool santa_awake = false;

bool elves_at_santa = false;
pthread_mutex_t elves_at_santa_mutex = PTHREAD_MUTEX_INITIALIZER;


/// Elves mutexes
int n_elves_waiting = 0;
pthread_mutex_t n_elves_waiting_mutex = PTHREAD_MUTEX_INITIALIZER;

bool elves_waiting_free = true;
pthread_mutex_t elves_waiting_free_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t elves_waiting_free_cond = PTHREAD_COND_INITIALIZER;

bool elves_ready[ALL_ELVES];
pthread_mutex_t elves_ready_mutex[ALL_ELVES];
pthread_cond_t elves_ready_cond[ALL_ELVES];

int waiting_elves_names[ELVES_WAITING];


/// Reindeer mutexes
int waiting_reindeer = 0;
pthread_mutex_t waiting_reindeer_mutex = PTHREAD_MUTEX_INITIALIZER;

bool all_reindeer_ready = false;
pthread_mutex_t all_reindeer_ready_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t all_reindeer_ready_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t reindeer_ready_mutex[ALL_REINDEER];
pthread_cond_t reindeer_ready_cond[ALL_REINDEER];
bool reindeer_ready[ALL_REINDEER];




void* elf_thread(void* arg){
    Elf *elf = (Elf *) arg;
    srand(pthread_self() * time(NULL));

    pthread_mutex_init(&elves_ready_mutex[elf->name], NULL);
    pthread_cond_init(&elves_ready_cond[elf->name], NULL);
    elves_ready[elf->name] = true;

    print("hi, im elf      %d %s", elf->name, elf_name(elf->name));

    while (true){
        lock_mutex(elves_ready_mutex[elf->name]);
        while (elves_ready[elf->name] == false){
            pthread_cond_wait(&elves_ready_cond[elf->name], &elves_ready_mutex[elf->name]);
        }
        elves_ready[elf->name] = false;
        unlock_mutex(elves_ready_mutex[elf->name]);

        sleep_seconds(ELF_WORKING_TIME);

        lock_mutex(elves_waiting_free_mutex);
        while (elves_waiting_free == false){
            pthread_cond_wait(&elves_waiting_free_cond, &elves_waiting_free_mutex);
        }

        lock_mutex(n_elves_waiting_mutex);
        if (n_elves_waiting == 2)
            elves_waiting_free = false;

        unlock_mutex(elves_waiting_free_mutex);


        waiting_elves_names[n_elves_waiting] = elf->name;
        n_elves_waiting ++;

        print("ELF %d %-20s, waiting %d", elf->name, elf_name(elf->name), n_elves_waiting);

        if (n_elves_waiting == 3){
            print("ELF %d %-20s--> Santa, wake up!", elf->name, elf_name(elf->name));

            lock_mutex(elves_at_santa_mutex);
            elves_at_santa = true;

            lock_mutex(santa_awake_mutex);
            santa_awake = true;
            pthread_cond_broadcast(&santa_awake_cond);
            unlock_mutex(santa_awake_mutex);

            unlock_mutex(elves_at_santa_mutex);

        }
        unlock_mutex(n_elves_waiting_mutex);
    }
    pthread_exit(NULL);
}




void* reindeer_thread(void* arg){
    Reindeer *reindeer = (Reindeer *) arg;
    srand(pthread_self() * time(NULL));

    pthread_mutex_init(&reindeer_ready_mutex[reindeer->name], NULL);
    pthread_cond_init(&reindeer_ready_cond[reindeer->name], NULL);
    reindeer_ready[reindeer->name] = true;

    print("hi, im reindeer %d %s", reindeer->name, reindeer_name(reindeer->name));

    while (true){
        lock_mutex(reindeer_ready_mutex[reindeer->name]);
        while (reindeer_ready[reindeer->name] == false){
            pthread_cond_wait(&reindeer_ready_cond[reindeer->name], &reindeer_ready_mutex[reindeer->name]);
        }
        reindeer_ready[reindeer->name] = false;
        unlock_mutex(reindeer_ready_mutex[reindeer->name]);

        sleep_seconds(REINDEER_HOLIDAY_TIME);

        lock_mutex(waiting_reindeer_mutex);
        waiting_reindeer += 1;

        print("reindeer %d %-15s, waiting: %d", reindeer->name, reindeer_name(reindeer->name), waiting_reindeer);
        if (waiting_reindeer == ALL_REINDEER){
            print("reindeer %d %-15s--> Santa, wake up!", reindeer->name, reindeer_name(reindeer->name));

            lock_mutex(all_reindeer_ready_mutex);
            all_reindeer_ready = true;

            lock_mutex(santa_awake_mutex);
            santa_awake = true;
            pthread_cond_broadcast(&santa_awake_cond);
            unlock_mutex(santa_awake_mutex);

            unlock_mutex(all_reindeer_ready_mutex);

            waiting_reindeer = 0;
        }

        unlock_mutex(waiting_reindeer_mutex);
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

        lock_mutex(all_reindeer_ready_mutex);
        if (all_reindeer_ready) {
            all_reindeer_ready = false;
            print("SANTA DELIVERING GIFTS", NULL);
            sleep_seconds(GIFTS_DELIVERY_TIME);

            int reindeer;
            for___(ALL_REINDEER, reindeer) {
                lock_mutex(reindeer_ready_mutex[reindeer]);
                reindeer_ready[reindeer] = true;
                pthread_cond_broadcast(&reindeer_ready_cond[reindeer]);
                unlock_mutex(reindeer_ready_mutex[reindeer]);
            }
        }
        unlock_mutex(all_reindeer_ready_mutex);

        lock_mutex(elves_at_santa_mutex);

        if(elves_at_santa){
            elves_at_santa = false;
            print("SANTA SOLVING PROBLEMS", NULL);
            sleep_seconds(PROBLEM_SOLVING_TIME);

            int elf;

            for___(ELVES_WAITING, elf){
                lock_mutex(elves_ready_mutex[waiting_elves_names[elf]]);
                elves_ready[waiting_elves_names[elf]] = true;
                pthread_cond_broadcast(&elves_ready_cond[waiting_elves_names[elf]]);
                unlock_mutex(elves_ready_mutex[waiting_elves_names[elf]]);
            }

            lock_mutex(n_elves_waiting_mutex);
            n_elves_waiting = 0;
            unlock_mutex(n_elves_waiting_mutex);

            lock_mutex(elves_waiting_free_mutex);
            elves_waiting_free = true;

            pthread_cond_broadcast(&elves_waiting_free_cond);
            unlock_mutex(elves_waiting_free_mutex);
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

    destroy_mutexes();
    return 0;
}




void destroy_mutexes(){
    pthread_mutex_destroy(&santa_awake_mutex);
    pthread_cond_destroy(&santa_awake_cond);
    pthread_mutex_destroy(&elves_at_santa_mutex);




    pthread_mutex_destroy(&n_elves_waiting_mutex);
    pthread_mutex_destroy(&elves_waiting_free_mutex);
    pthread_cond_destroy(&elves_waiting_free_cond);

    for_i(ALL_ELVES){
        pthread_mutex_destroy(&elves_ready_mutex[i]);
        pthread_cond_destroy(&elves_ready_cond[i]);
    }



    pthread_mutex_destroy(&waiting_reindeer_mutex);
    pthread_mutex_destroy(&all_reindeer_ready_mutex);
    pthread_cond_destroy(&all_reindeer_ready_cond);

    for_i(ALL_REINDEER){
        pthread_mutex_destroy(&reindeer_ready_mutex[i]);
        pthread_cond_destroy(&reindeer_ready_cond[i]);
    }
}

#pragma clang diagnostic pop