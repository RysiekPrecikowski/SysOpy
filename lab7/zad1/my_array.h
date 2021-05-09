#ifndef SYSOPY_MY_ARRAY_H
#define SYSOPY_MY_ARRAY_H

#include "utils.h"

#define MAX_OVEN_PIZZAS 5
#define MAX_TABLE_PIZZAS 5
#define MAX_PIZZAS (MAX_OVEN_PIZZAS > MAX_TABLE_PIZZAS ? MAX_OVEN_PIZZAS : MAX_TABLE_PIZZAS)

#define PIZZA_TYPES_COUNT 10

#define PREPARATION_TIME (rand() % 2 + 1)
#define BAKING_TIME (rand() % 3 + 4)
#define DELIVERY_TIME (rand() % 3 + 4)


#define EMPTY (-1)

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

#endif //SYSOPY_MY_ARRAY_H
