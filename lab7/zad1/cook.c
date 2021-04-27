#include "my_shared_memory.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
my_array *table;
my_array *oven;
int sem;

int main(int argc, string argv[]){
    int shared_memory_id;
    shared_memory *sharedMemory =
            set_up_shared_memory( 0666, 0666,
                                 &shared_memory_id, 0);
    sem = sharedMemory->sem;

    table = &sharedMemory->table;
    oven = &sharedMemory->oven;

    print("\nW KUCHARZU :O")
    srand(time(NULL));



    while (true){
        int pizza_type = rand() % PIZZA_TYPES_COUNT;
        print("%d preparing pizza %d", getpid(), pizza_type);
        print_current_time();
        sleep_seconds(PREPARATION_TIME);

        wait_semaphore_0(sem, OVEN);
        change_semaphore_value(sem, OVEN, 1);
        print("%d placing pizza %d in oven", getpid(), pizza_type);
        print_current_time();
        add_to_array(oven, pizza_type);
        change_semaphore_value(sem, OVEN, -1);

        print("%d is baking pizza %d", getpid(), pizza_type);
        print_current_time();
        sleep_seconds(BAKING_TIME);


        wait_semaphore_0(sem, OVEN);
        change_semaphore_value(sem, OVEN, 1);
        print("%d is taking pizza %d from oven", getpid(), pizza_type)
        print_current_time();
        int taken_pizza = take_from_array(oven);
        change_semaphore_value(sem, OVEN, -1);

        //TODO co gdy piec lub stół sie zapelni???

        wait_semaphore_0(sem, TABLE);

        change_semaphore_value(sem, TABLE, 1);
        print("%d is placing pizza %d on table", getpid(), taken_pizza);
        print_current_time();
        add_to_array(table, taken_pizza);
        change_semaphore_value(sem, TABLE, -1);
    }




    return 0;
}
#pragma clang diagnostic pop