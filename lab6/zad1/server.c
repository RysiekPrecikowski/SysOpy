#include "my_messages.h"

int clients_q[MAX_CLIENTS];
int clients_status[MAX_CLIENTS];
int clients_ids[MAX_CLIENTS];
int clients_counter = 0;

int server_queue;

void close_server_queue(){
    close_queue(server_queue);
}
void handle_sigint(){
    close_server_queue();
    eprint("exiting SIGINT");
    exit(0);
}

int get_client_ind(int id){
    for_i(MAX_CLIENTS){
        if (clients_ids[i] == id)
            return i;
    }
    return NO_SUCH_ID;
}

int get_client_q(int id){
    int ind = get_client_ind(id);
    if (ind != NO_SUCH_ID)
        return clients_q[ind];
    return NO_SUCH_ID;
}

int get_client_status(int id){
    int ind = get_client_ind(id);
    if (ind != NO_SUCH_ID)
        return clients_status[ind];
    return NO_SUCH_ID;
}

int get_free_id(){
    for_i(MAX_CLIENTS){
        if (clients_status[i] == NOT_CONNECTED){
            return i;
        }
    }
    return NO_FREE_INDEX;
}


void got_stop(message *m){
    print("GOT STOP :o %d ", m->sender_id);
    int client_ind = get_client_ind(m->sender_id);
    if (client_ind == NO_SUCH_ID){
        eprint("WRONG ID index: %d", client_ind);
    }

    clients_status[client_ind] = NOT_CONNECTED;
}

void got_disconnect(message *m){
    print("GOT DISCONNECT :o %d ", m->sender_id);
    int client_ind = get_client_ind(m->sender_id);

    if (client_ind == NO_SUCH_ID){
        eprint("WRONG ID index: %d", client_ind);
    }

    int *connection_status = &clients_status[client_ind];

    if (*connection_status != BUSY){
        eprint("CLIENT IS NOT BUSY!!!! status %s", status_to_str(*connection_status));
    }

    *connection_status = READY;
}

void got_list(message *m){
    print("*******   CLIENTS   *******");
    print(" ");
    for_i(MAX_CLIENTS){
        int status = clients_status[i];
        if (status == NOT_CONNECTED) {
            print("ind %d -> STATUS: %s", i, status_to_str(status));
        } else {
            print("ind %d -> STATUS: %s   ID: %d   QUEUE: %d", i, status_to_str(status), clients_ids[i], clients_q[i]);
        }
    }
    print(" \n***************************\n");
}

void got_connect(message *m) {
    print("GOT CONNECT :o %d with %d", m->sender_id, m->client_id);

    int connect_ids[] = {m->sender_id, m->client_id};

    int connect_inds[] = {get_client_ind(connect_ids[0]),
                          get_client_ind(connect_ids[1])};

    if (connect_inds[0] == NO_SUCH_ID or connect_inds[1] == NO_SUCH_ID){
        eprint("WRONG ID indexes: %d %d", connect_inds[0], connect_inds[1]);
    }

    int *connect_statuses[] = {&clients_status[connect_inds[0]],
                               &clients_status[connect_inds[1]]};


    if (*connect_statuses[0] != READY or *connect_statuses[1] != READY){
        eprint("CLIENT IS BUSY!! statuses: %s %s", status_to_str(*connect_statuses[0])
                                                 , status_to_str(*connect_statuses[1]));
    }

    message to_send = {.mtype = CONNECT, .sender_id = SERVER_ID};

    for_i(2) {
        to_send.queue = clients_q[connect_inds[(i + 1)%2]];
        send_message(clients_q[connect_inds[i]], to_send, 0);
        *connect_statuses[i] = BUSY;
    }
}

void got_init(message *m){
    int client_ind = get_free_id();
    print("GOT INIT :O %d", m->queue);

    if (client_ind == NO_FREE_INDEX){
        eprint("not enough space in server");
    }
    int client_id = clients_counter;
    int client_queue = m->queue;

    clients_q[client_ind] = client_queue;
    clients_status[client_ind] = READY;
    clients_ids[client_ind] = client_id;

    message to_client_message = {.mtype = INIT,
                                 .sender_id = SERVER_ID,
                                 .client_id = client_id};

    send_message(client_queue, to_client_message, 0);

    clients_counter += 1;
}


#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main(void){
    signal(SIGINT, handle_sigint);
//    atexit(close_server_queue);

    server_queue = create_queue(HOME, ID, IPC_CREAT |  0666);
    print("SERVER QUEUE IN SERVER: %d", server_queue);
    set_array(clients_status, MAX_CLIENTS, NOT_CONNECTED);

    bool testing = true;
    testing = false;
    if (testing) {
        message test = {.mtype = INIT, .queue = 666};
        send_message(server_queue, test, 0);
        test.queue = 555;
        send_message(server_queue, test, 0);

        test.mtype = LIST;
        send_message(server_queue, test, 0);

        test.mtype = CONNECT;
        test.sender_id = 0;
        test.client_id = 1;
        send_message(server_queue, test, 0);

        test.mtype = LIST;
        send_message(server_queue, test, 0);


        test.mtype = DISCONNECT;
        test.sender_id = 0;
        send_message(server_queue, test, 0);

        test.mtype = LIST;
        send_message(server_queue, test, 0);

        test.mtype = STOP;
        test.sender_id = 1;
        send_message(server_queue, test, 0);

        test.mtype = LIST;
        send_message(server_queue, test, 0);

        test.mtype = INIT;
        test.queue = 444;
        send_message(server_queue, test, 0);

        test.mtype = LIST;
        send_message(server_queue, test, 0);

    }
    message received;


    while (true){
        receive_message(server_queue, 0, &received); //teraz zwykla kolejka
//        receive_message(server_queue, -ALL_TYPES, &received); //TODO chyba tak ma byc

        switch (received.mtype) {
            case STOP:
                got_stop(&received);
                break;
            case DISCONNECT:
                got_disconnect(&received);
                break;
            case LIST:
                got_list(&received);
                break;
            case CONNECT:
                got_connect(&received);
                break;
            case INIT:
                got_init(&received);
                break;
            default:
                break;
        }
    }

    return 0;
}
#pragma clang diagnostic pop