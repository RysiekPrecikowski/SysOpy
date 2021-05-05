#include "my_shared_memory.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
my_array *table;
my_array *oven;
int sem;
//int sem_delivery;




int main(int argc, string argv[]){
    int shared_memory_id;
    shared_memory *sharedMemory =
            set_up_shared_memory( 0666, 0666,
                                 &shared_memory_id, 0);
    sem = sharedMemory->sem;

    table = &sharedMemory->table;
    oven = &sharedMemory->oven;

    srand(getpid() * time(NULL));
    while (true){
        int pizza_type = rand() % PIZZA_TYPES_COUNT;
        print("%d preparing pizza %d", getpid(), pizza_type);
        print_current_time();
        sleep_seconds(PREPARATION_TIME);


        wait_semaphore_0(sem, OVEN);
        change_semaphore_value(sem, OVEN, 1);
        print("%d placing pizza %d in oven", getpid(), pizza_type);
        add_to_array(oven, pizza_type);
//        print_my_array(oven);
        print_current_time();
        change_semaphore_value(sem, OVEN, -1);

        print("%d is baking pizza %d", getpid(), pizza_type);
        print_current_time();
        sleep_seconds(BAKING_TIME);


        wait_semaphore_0(sem, OVEN);
        change_semaphore_value(sem, OVEN, 1);
//        print_my_array(oven);
        int taken_pizza = take_from_array(oven);
        print("%d is taking pizza %d from oven, should %d", getpid(), taken_pizza, pizza_type)
        print_current_time();
        change_semaphore_value(sem, OVEN, -1);


//        get_semaphore_value(sem, OVEN);
//        get_semaphore_value(sem, TABLE);
//        get_semaphore_value(sem, TABLE_DELIVERY);


        wait_semaphore_0(sem, TABLE);
        change_semaphore_value(sem, TABLE_DELIVERY_PUT, -1);
        change_semaphore_value(sem, TABLE, 1);
        print("%d is placing pizza %d on table", getpid(), taken_pizza);
        print_current_time();
        add_to_array(table, taken_pizza);
//        print_my_array(table);
        change_semaphore_value(sem, TABLE, -1);
        change_semaphore_value(sem, TABLE_DELIVERY_TAKE, 1);

    }

    return 0;
}
#pragma clang diagnostic pop