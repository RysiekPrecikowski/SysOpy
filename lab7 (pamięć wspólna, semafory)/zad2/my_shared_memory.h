#ifndef SYSOPY_MY_SHARED_MEMORY_H
#define SYSOPY_MY_SHARED_MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include <semaphore.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "my_array.h"
#include "utils.h"



#define LOCKED (-1)
#define UNLOCKED 1

#define ADD_PIZZA 1
#define TAKE_PIZZA (-1)

#define SH_NAME "/shared"


enum SEMAPHORES {TABLE, OVEN, TABLE_DELIVERY_TAKE, TABLE_DELIVERY_PUT, ALL_SEMAPHORES};

typedef struct {
    my_array oven;
    my_array table;
}shared_memory;


#define SIZE_OF_SHARED_MEMORY (sizeof(shared_memory))

char* semaphore_to_string(int sem){
    switch (sem) {
        case OVEN:
            return (char*) "OVEN";
        case TABLE:
            return (char*) "TABLE";
        case TABLE_DELIVERY_PUT:
            return (char*) "TABLE_DELIVERY_PUT";
        case TABLE_DELIVERY_TAKE:
            return (char*) "TABLE_DELIVERY_TAKE";
    }
    return NULL;
}


shared_memory* set_up_shared_memory(int flag_open, int flag_map){
    int id = shm_open(SH_NAME, flag_open, 0666);
    if (id == -1)
        eprint("ERROR IN OPENING SHARED MEMORY");

    if (ftruncate(id, SIZE_OF_SHARED_MEMORY) == -1)
        eprint("ERROR IN TRUNCATING SHARED MEMORY");

    shared_memory *memory = mmap(NULL, SIZE_OF_SHARED_MEMORY, flag_map, MAP_SHARED, id, 0);

    if (memory == (void *) -1){
        eprint("ERROR IN MAPPING SHARED MEMORY");
    }
    return memory;
}

void delete_shared_memory(){
    if (shm_unlink(SH_NAME) == -1) {
        eprint("ERROR IN UNLINKING MEMORY");
    }
}

void close_shared_memory(shared_memory *sharedMemory){
    if (munmap(sharedMemory, SIZE_OF_SHARED_MEMORY) == -1)
        eprint("ERROR IN UNMAPPING");
}

char* get_sem_path(int sem){
    char *path = calloc(strlen(semaphore_to_string(sem) + 2), sizeof (char ));
    sprintf(path, "/%s", semaphore_to_string(sem));
    return path;
}

sem_t** create_semaphores(){
    sem_t ** semaphores = calloc(ALL_SEMAPHORES, sizeof(sem_t*));
    int starting_values[ALL_SEMAPHORES];

    starting_values[TABLE] = UNLOCKED;
    starting_values[OVEN] = UNLOCKED;
    starting_values[TABLE_DELIVERY_PUT] = MAX_TABLE_PIZZAS;
    starting_values[TABLE_DELIVERY_TAKE] = 0;

    for_i(ALL_SEMAPHORES) {
        char *path = get_sem_path(i);

        semaphores[i] = sem_open(path, O_CREAT, 0666, starting_values[i]);
        if (semaphores[i] == SEM_FAILED){
            eprint("ERROR IN CREATING SEMAPHORE, %s", strerror(errno));
        }
    }
    return semaphores;
}

sem_t** get_semaphores(){
    sem_t** semaphores = calloc(ALL_SEMAPHORES, sizeof(int));
    for_i(ALL_SEMAPHORES) {
        char* path = get_sem_path(i);

        semaphores[i] = sem_open(path, O_RDWR);
        if (semaphores[i] == SEM_FAILED){
            eprint("ERROR IN CREATING SEMAPHORE, %s", strerror(errno));
        }
    }
    return semaphores;
}

int get_semaphore_value(sem_t *sem[ALL_SEMAPHORES], int n){
    int val;
    if (sem_getvalue(sem[n], &val) == -1){
        eprint("error when getting value, %s", strerror(errno));
    }
    print("sem %d %s value: %d", n, semaphore_to_string(n), val);
    return val;
}


void change_semaphore_value(sem_t *sem[ALL_SEMAPHORES], int n, int diff){
    switch (diff) {
        case -1:
            if (sem_wait(sem[n]) == -1)
                eprint("ERROR IN LOCKING, %s", strerror(errno));
            break;
        case 1:
            if (sem_post(sem[n]) == -1)
                eprint("ERROR IN LOCKING, %s", strerror(errno));
            break;
        default: eprint("UNSUPPORTED OPERATION");
    }
}

void delete_semaphores(){
    for_i(ALL_SEMAPHORES){
        if (sem_unlink(get_sem_path(i)) == -1){
            eprint("ERROR IN UNLINKING SEMAPHORE");
        }
    }
}

void close_semaphores(sem_t** semaphores){
    for_i(ALL_SEMAPHORES){
        if(sem_close(semaphores[i]) == -1){
            eprint("ERROR IN CLOSING SEMAPHORE, %s", strerror(errno));
        }
    }
}


void handle_sigint(int sig){
    exit(-1);
}
#endif //SYSOPY_MY_SHARED_MEMORY_H
