#include "my_shared_memory.h"

my_array *table;
my_array *oven;

int main(void){
    int shared_memory_id;
    shared_memory *sharedMemory =
            set_up_shared_memory(IPC_CREAT | 0666, 0666,
                                 &shared_memory_id, SIZE_OF_SHARED_MEMORY);


    table = &sharedMemory->table;
    oven = &sharedMemory->oven;

    set_my_arr(table, MAX_TABLE_PIZZAS);
    set_my_arr(oven, MAX_OVEN_PIZZAS);



    print("w mainie:");

    add_to_array(table, 666);
    print_my_array(table);



    exec("cook", NULL);

    wait_all_children

    print("\nsprawdzam czy w mainie sie zmienilo");
    print_my_array(table);
    return 0;
}