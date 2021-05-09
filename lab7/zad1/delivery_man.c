#include "my_shared_memory.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int sem;
int shared_memory_id;
shared_memory *sharedMemory;

void bye(){
    detach_shared_memory(sharedMemory);
}

int main(int argc, string argv[]){
    atexit(bye);
    signal(SIGINT, handle_sigint);

    sharedMemory = set_up_shared_memory( 0666, 0666,
                                         &shared_memory_id, 0);
    sem = get_n_semaphores(ALL_SEMAPHORES, 0666);

    my_array *table = &sharedMemory->table;
    my_array *oven = &sharedMemory->oven;

    srand(getpid() * time(NULL));

    while (true){

        change_semaphore_value(sem, TABLE_DELIVERY_TAKE, TAKE_PIZZA); // zabieram miejsce z "do zabrania"
        change_semaphore_value(sem, TABLE, LOCKED);

        print("OVEN")
        print_my_array(oven);
        print("TABLE")
        print_my_array(table);

        int taken_pizza = take_from_array(table);
        print("DELIVERY %d is taking pizza %d from table", getpid(), taken_pizza);
        print_current_time();

        change_semaphore_value(sem, TABLE, UNLOCKED);
        change_semaphore_value(sem, TABLE_DELIVERY_PUT, ADD_PIZZA); // dodaje jedno miejsce "do polozenia"


        sleep_seconds(DELIVERY_TIME);

        print("DELIVERY %d delivered pizza %d", getpid(), taken_pizza);
        print_current_time();

        sleep_seconds(DELIVERY_TIME);
    }




    return 0;
}
#pragma clang diagnostic pop