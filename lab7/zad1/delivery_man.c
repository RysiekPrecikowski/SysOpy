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

    print("\nW DOSTAWCY :O")
    srand(time(NULL));



    while (true){
        wait_my_array_not_empty(table);
        wait_semaphore_0(sem, TABLE);
        change_semaphore_value(sem, TABLE, 1);
        int taken_pizza = take_from_array(table);
        print("%d is taking pizza %d from table", getpid(), taken_pizza);
        print_current_time();
        change_semaphore_value(sem, TABLE, -1);

        sleep_seconds(DELIVERY_TIME);

        print("%d delivered pizza %d", getpid(), taken_pizza);
        print_current_time();

        sleep_seconds(DELIVERY_TIME);
    }




    return 0;
}
#pragma clang diagnostic pop