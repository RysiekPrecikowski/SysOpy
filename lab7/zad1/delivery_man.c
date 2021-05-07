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

        change_semaphore_value(sem, TABLE_DELIVERY_TAKE, TAKE_PIZZA);
        change_semaphore_value(sem, TABLE, LOCKED);

        print("OVEN")
        print_my_array(oven);
        print("TABLE")
        print_my_array(table);

        int taken_pizza = take_from_array(table);
        print("DELIVERY %d is taking pizza %d from table", getpid(), taken_pizza);
        print_current_time();

        change_semaphore_value(sem, TABLE, UNLOCKED);
        change_semaphore_value(sem, TABLE_DELIVERY_PUT, ADD_PIZZA);


        sleep_seconds(DELIVERY_TIME);

        print("DELIVERY %d delivered pizza %d", getpid(), taken_pizza);
        print_current_time();

        sleep_seconds(DELIVERY_TIME);
    }




    return 0;
}
#pragma clang diagnostic pop