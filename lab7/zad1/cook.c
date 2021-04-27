#include "my_shared_memory.h"

my_array *table;
my_array *oven;

int main(int argc, string argv[]){
    int shared_memory_id;
    shared_memory *sharedMemory =
            set_up_shared_memory( 0666, 0666,
                                 &shared_memory_id, 0);

    table = &sharedMemory->table;
    oven = &sharedMemory->oven;


    print("\nW KUCHARZU :O")

    print_my_array(table);

    print("\nDODAJE W KUCHARZU 555");

    add_to_array(table, 555);

    return 0;
}