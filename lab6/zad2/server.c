#include "my_messages.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int clients_q[MAX_CLIENTS];
int clients_status[MAX_CLIENTS];
char clients_paths[MAX_CLIENTS][MAX_MESSAGE_LEN];
int clients_ids[MAX_CLIENTS];
int clients_counter = 0;

int server_queue;

void close_server(){
    for_i(MAX_CLIENTS){
        if (clients_status[i] != NOT_CONNECTED){
            message to_send = {.mtype = STOP, .sender_id = SERVER_ID};
            send_message(clients_q[i], to_send);
            print("SENT STOP TO CLIENT ind: %d, id: %d", i, clients_ids[i]);

//            message received;
//            receive_message(server_queue, STOP, received);
//            print("RECEIVED STOP FROM CLIENT");
        }
    }

    close_queue(server_queue);
}
void handle_sigint(){
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

    message to_send = {.mtype = STOP, .sender_id = SERVER_ID};
    print("SENDING STOP TO CLIENT")
    send_message(clients_q[client_ind], to_send);
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
        print("SENDING PATH: %s", clients_paths[connect_inds[(i+1) %2]])
        sprintf(to_send.message, "%s", clients_paths[connect_inds[(i+1) %2]]);

        send_message(clients_q[connect_inds[i]], to_send);
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
    int client_queue = mq_open(m->message, O_WRONLY);
    print("OPENED QUEUE %d", client_queue)

    clients_q[client_ind] = client_queue;
    clients_status[client_ind] = READY;
    clients_ids[client_ind] = client_id;
//    clients_paths[client_ind] = m->message;
    sprintf(clients_paths[client_ind], "%s", m->message);

    print("sending init to client, %s", m->message);

    message to_client_message = {.mtype = INIT,
            .sender_id = SERVER_ID,
            .client_id = client_id};


    send_message(client_queue, to_client_message);

    clients_counter += 1;
}



int main(void){
    signal(SIGINT, handle_sigint);
    atexit(close_server);

    server_queue = create_server_queue();
    print("SERVER QUEUE: %d", server_queue);
    set_array(clients_status, MAX_CLIENTS, NOT_CONNECTED);

    message received;
    int priority = 0;

    while (true){
        receive_message(server_queue, priority, received);

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