#include "my_shared_memory.h"

my_array *table;
my_array *oven;
int sem_cook;
int sem_delivery;
int shared_memory_id;

void bye(){
    delete_shared_memory(shared_memory_id);
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

    sharedMemory->sem_cook = get_n_semaphores(ALL_SEMAPHORES ,IPC_CREAT | 0666);
    sem_cook = sharedMemory->sem_cook;
    set_semaphore_value(sem_cook, OVEN, 0);
    set_semaphore_value(sem_cook, TABLE, 0);
    set_semaphore_value(sem_cook, TABLE_DELIVERY, 0);

//    sharedMemory->sem_delivery = get_n_semaphores(ALL_SEMAPHORES ,IPC_CREAT | 0666);
//    sem_delivery = sharedMemory->sem_delivery;
//    set_semaphore_value(sem_delivery, TABLE, 0);
//    set_semaphore_value(sem_delivery, OVEN, 0);



    print("w mainie:");
//

//
//    for_i(MAX_TABLE_PIZZAS * 2+2){
//
//        if (i % 2 == 0 and i > 0){
//            print("Taking %d", take_from_array(table));
//        }
//        print("adding %d", i);
//        add_to_array(table, i);
//
//        print_my_array(table);
//    }


    print_current_time();



    exec("cook", NULL);
    sleep_seconds(0.3);
    exec("cook", NULL);

    exec("delivery_man", NULL);
    exec("delivery_man", NULL);
    exec("delivery_man", NULL);





    wait_all_children;

//    print("\nsprawdzam czy w mainie sie zmienilo");
//    print_my_array(table);
//

    return 0;
}