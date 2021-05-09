#include "my_shared_memory.h"

sem_t **sem;
int shared_memory_id;
shared_memory *sharedMemory;


void bye(){
    close_shared_memory(sharedMemory);
    delete_shared_memory();
    close_semaphores(sem);
    delete_semaphores();
}

int main(void){
    signal(SIGINT, handle_sigint);
    atexit(bye);

    my_array *table;
    my_array *oven;

    sharedMemory = set_up_shared_memory(O_CREAT | O_RDWR, PROT_READ | PROT_WRITE);

    table = &sharedMemory->table;
    oven = &sharedMemory->oven;

    set_my_arr(table, MAX_TABLE_PIZZAS);
    set_my_arr(oven, MAX_OVEN_PIZZAS);

    sem = create_semaphores();

    for_i(2)
        exec("cook", NULL);

    for_i(2)
        exec("delivery_man", NULL);


    wait_all_children;

    return 0;
}