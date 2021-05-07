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

#include "utils.h"

#define MAX_OVEN_PIZZAS 5
#define MAX_TABLE_PIZZAS 5
#define MAX_PIZZAS (MAX_OVEN_PIZZAS > MAX_TABLE_PIZZAS ? MAX_OVEN_PIZZAS : MAX_TABLE_PIZZAS)

#define PIZZA_TYPES_COUNT 10

#define PREPARATION_TIME (rand() % 2 + 1)
#define BAKING_TIME (rand() % 3 + 4)
#define DELIVERY_TIME (rand() % 3 + 4)

#define EMPTY (-1)

#define LOCKED (-1)
#define UNLOCKED 1

#define ADD_PIZZA 1
#define TAKE_PIZZA (-1)

#define PROJECT_ID 'X'
#define HOME getenv("HOME")


enum SEMAPHORES {TABLE, OVEN, TABLE_DELIVERY_TAKE, TABLE_DELIVERY_PUT, ALL_SEMAPHORES};


typedef struct my_array {
    int adding_counter;
    int taking_counter;
    int counter;
    int size;
    int arr[MAX_PIZZAS];
}my_array;

int size_of_my_arr(my_array *arr){
    return sizeof (my_array) + arr->size * sizeof(arr[0]);
}


typedef struct {
    int sem;
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



void set_my_arr(my_array *arr, int size){
    arr->size = size;
    arr->adding_counter = 0;
    arr->taking_counter = 0;
    arr->counter = 0;
    set_array(arr->arr, size, EMPTY);
}

void add_to_array(my_array *arr, int to_add) {
    if (arr->counter == arr->size) {
        eprint("ARRAY FULL")
//        my_exit(-1);
    } else if (arr->arr[arr->adding_counter % arr->size] != EMPTY) {
        eprint("ARRAY NOT EMPTY BUT INDEX TAKEN")
//        my_exit(-2);
    } else {
        arr->arr[arr->adding_counter % arr->size] = to_add;
        arr->adding_counter ++;
        arr->counter ++;
    }
}

int take_from_array(my_array *arr){
    if (arr->arr[arr->taking_counter % arr->size] == EMPTY){
        eprint("NO ITEM")
//        my_exit(-3);
    } else {
        int res = arr->arr[arr->taking_counter % arr->size];
        arr->arr[arr->taking_counter % arr->size] = EMPTY;
        arr->taking_counter += 1;
        arr->counter --;
        return res;
    }
    return -1;
}


void wait_my_array_not_empty(my_array *arr){
    while (arr->counter == 0);
}

void wait_my_array_not_full(my_array *arr){
    while (arr->size == arr->counter);
}

void print_my_array(my_array *arr){
    print("taken: %d / %d", arr->counter, arr->size);
//    print_array(arr->arr, arr->size, "%d ");

    for_i(arr->counter){
        printf("%d ", arr->arr[(arr->taking_counter + i) % arr->size]);
    }
    printf("\n");
}


#endif //SYSOPY_MY_SHARED_MEMORY_H
