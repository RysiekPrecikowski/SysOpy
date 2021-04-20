#include "my_messages.h"

int server_queue;
int client_queue;
int client_id;
int second_client_queue;

void init(){
    message m = {.mtype = INIT, .queue = client_queue};
    send_message(server_queue, m, 0);
    print("SENDING INIT");

    receive_message(client_queue, INIT, &m);
    print("RECEIVED MESSAGE")
    client_id = m.client_id;
    print("CLIENT ID: %d", client_id);
}

void connect(int connect_to_id){
    message m = {.mtype = CONNECT, .sender_id = client_id, .client_id = connect_to_id};
    send_message(server_queue, m, 0);

    receive_message(client_queue, CONNECT, &m);

    second_client_queue = m.queue;
    print("SECOND CLIENT QUEUE: %d", second_client_queue);
}

void disconnect(){
    message m = {.mtype = DISCONNECT, .sender_id = client_id};
    send_message(server_queue, m, 0);
}

void list(){
    message m = {.mtype = LIST};
    send_message(server_queue, m, 0);
}

int main(void){
    server_queue = create_queue(HOME, ID, 0);
    print("server queue %d", server_queue);

    client_queue = create_queue(HOME, CLIENT_ID, IPC_CREAT |  0666);
    print("client queue %d", client_queue);

    init();
//    connect(0);
//    disconnect();
    list();
    return 0;
}