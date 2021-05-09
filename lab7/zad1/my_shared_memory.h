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
#include<sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "my_array.h"
#include "utils.h"



#define LOCKED (-1)
#define UNLOCKED 1

#define ADD_PIZZA 1
#define TAKE_PIZZA (-1)

#define PROJECT_ID 'X'
#define HOME getenv("HOME")


enum SEMAPHORES {TABLE, OVEN, TABLE_DELIVERY_TAKE, TABLE_DELIVERY_PUT, ALL_SEMAPHORES};

typedef struct {
    my_array oven;
    my_array table;
}shared_memory;


#define SIZE_OF_SHARED_MEMORY (sizeof(shared_memory))


shared_memory* set_up_shared_memory(int flag_get, int flag_mat, int* shared_memory_id, int size_of_memory){
    int key = ftok(HOME, PROJECT_ID);
    *shared_memory_id = shmget(key, size_of_memory, flag_get);
    if (*shared_memory_id == -1){
        eprint("ERROR IN SET UP SHARED MEMORY %s", strerror(errno))
    }
    return (shared_memory*) shmat(*shared_memory_id, NULL, flag_mat);
}

void delete_shared_memory(int id){
    shmctl(id, IPC_RMID, NULL);
}

void detach_shared_memory(shared_memory* sharedMemory){
    if (shmdt(sharedMemory) == -1){
        eprint("ERROR IN DETACHING SHARED MEMORY");
    }
}

int get_n_semaphores(int n, int flag){
    int key = ftok(HOME, PROJECT_ID);
    int id = semget(key, n, flag);
    if (id == -1){
        eprint("ERROR WHEN CREATING SEMAPHORE %s", strerror(errno));

    }
    return id;
}

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

int get_semaphore_value(int id, int n){
    int val = semctl(id, n, GETVAL, 0);
    print("sem %d %s value: %d", n, semaphore_to_string(n), val);
    return val;
}

void set_semaphore_value(int id, int n, int to_set){
    semctl(id, n, SETVAL, to_set);
}

void change_semaphore_value(int id, int n, int diff){
    if (diff == 0)
        eprint("DUDE DIFF CANNOT BE 0")
    struct sembuf sembuf = {.sem_num = n, .sem_op = diff};
    semop(id, &sembuf, 1);
//    print("value changed diff %d", diff);
}

void delete_semaphore(int id){
    semctl(id, 0, IPC_RMID, 0);
}

void wait_semaphore_0(int id, int n){
    struct sembuf sembuf = {.sem_num = n, .sem_op = 0};
    semop(id, &sembuf, 1);
}

void handle_sigint(int sig){
    exit(-1);
}

#endif //SYSOPY_MY_SHARED_MEMORY_H
