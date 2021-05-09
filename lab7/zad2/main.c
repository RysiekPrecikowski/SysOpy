#include "my_shared_memory.h"

sem_t ** sem;
int shared_memory_id;

void bye(){
    delete_shared_memory();
    delete_semaphores();
}

void handle_sigint(int sig){
    exit(-1);
}

int main(void){
    signal(SIGINT, handle_sigint);
    atexit(bye);


    my_array *table;
    my_array *oven;

    shared_memory *sharedMemory =
            set_up_shared_memory(O_CREAT | O_RDWR, PROT_READ | PROT_WRITE);

    table = &sharedMemory->table;
    oven = &sharedMemory->oven;

    set_my_arr(table, MAX_TABLE_PIZZAS);
    set_my_arr(oven, MAX_OVEN_PIZZAS);

    sem = create_semaphores();

//    add_to_array(table, 1);

//    exec("cook", NULL);
//    exec("delivery_man", NULL);

    for_i(2)
        exec("cook", NULL);

    for_i(2)
        exec("delivery_man", NULL);


    wait_all_children;

    return 0;
}