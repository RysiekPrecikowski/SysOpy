#include "my_shared_memory.h"

my_array *table;
my_array *oven;
int sem;
int shared_memory_id;

void bye(){
    delete_shared_memory(shared_memory_id);
    delete_semaphore(sem);
}

int main(void){
    atexit(bye);

    shared_memory *sharedMemory =
            set_up_shared_memory(IPC_CREAT | 0666, 0666,
                                 &shared_memory_id, SIZE_OF_SHARED_MEMORY);

    table = &sharedMemory->table;
    oven = &sharedMemory->oven;

    set_my_arr(table, MAX_TABLE_PIZZAS);
    set_my_arr(oven, MAX_OVEN_PIZZAS);

    sharedMemory->sem = get_n_semaphores(ALL_SEMAPHORES ,IPC_CREAT | 0666);
    sem = sharedMemory->sem;
    set_semaphore_value(sem, OVEN, 0);
    set_semaphore_value(sem, TABLE, 0);
    set_semaphore_value(sem, TABLE_DELIVERY_TAKE, 0);
    set_semaphore_value(sem, TABLE_DELIVERY_PUT, MAX_TABLE_PIZZAS);


    for_i(2)
        exec("cook", NULL);

    for_i(2)
        exec("delivery_man", NULL);


    wait_all_children;

    return 0;
}