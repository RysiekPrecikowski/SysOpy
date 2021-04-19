#include "my_messages.h"

int server_queue;

void init(){

}

void connect(int connect_to_id){

}

void disconnect(){

}

int main(void){
    server_queue = create_queue(HOME, ID, 0);
    print("%d", server_queue);

    return 0;
}