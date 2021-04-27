#include "my_shared_memory.h"

my_array *table;
my_array *oven;
int sem;

int main(void){

    int shared_memory_id;
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



    print("w mainie:");

//    add_to_array(table, 666);
    print_my_array(table);



    print_current_time();



    exec("cook", NULL);
    exec("delivery_man", NULL);






    wait_all_children

    print("\nsprawdzam czy w mainie sie zmienilo");
    print_my_array(table);

    delete_shared_memory(shared_memory_id);
    return 0;
}