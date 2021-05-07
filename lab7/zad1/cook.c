#include "my_shared_memory.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main(int argc, string argv[]){
    int shared_memory_id;
    shared_memory *sharedMemory =
            set_up_shared_memory( 0666, 0666,
                                  &shared_memory_id, 0);
    int sem = sharedMemory->sem;

    my_array *table = &sharedMemory->table;
    my_array *oven = &sharedMemory->oven;

    srand(getpid() * time(NULL));

    while (true){

        int pizza_type = rand() % PIZZA_TYPES_COUNT;
        print("%d preparing pizza %d", getpid(), pizza_type);
        print_current_time();
        sleep_seconds(PREPARATION_TIME);


        change_semaphore_value(sem, OVEN, LOCKED);
        print("%d placing pizza %d in oven", getpid(), pizza_type);
        add_to_array(oven, pizza_type);
        print_current_time();
        change_semaphore_value(sem, OVEN, UNLOCKED);

        print("%d is baking pizza %d", getpid(), pizza_type);
        print_current_time();
        sleep_seconds(BAKING_TIME);


        change_semaphore_value(sem, OVEN, LOCKED);
        int taken_pizza = take_from_array(oven);
        print("%d is taking pizza %d from oven, should %d", getpid(), taken_pizza, pizza_type)
        print_current_time();
        change_semaphore_value(sem, OVEN, UNLOCKED);


        change_semaphore_value(sem, TABLE_DELIVERY_PUT, TAKE_PIZZA);
        change_semaphore_value(sem, TABLE, LOCKED);

        print("%d is placing pizza %d on table", getpid(), taken_pizza);
        print_current_time();
        add_to_array(table, taken_pizza);

        change_semaphore_value(sem, TABLE, UNLOCKED);
        change_semaphore_value(sem, TABLE_DELIVERY_TAKE, ADD_PIZZA);

    }

    return 0;
}
#pragma clang diagnostic pop