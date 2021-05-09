#include "my_messages.h"
#include <linux/input.h>
#include <curses.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int server_queue;
int client_queue;
int client_id;
int second_client_queue;
bool chat_mode = false;

void close_client_queue(){
    close_queue(client_queue);
}

void stop(){
    print("SENDING STOP")
    message m = {.mtype = STOP, .sender_id = client_id};
    send_message(server_queue, m);

    receive_message(client_queue, STOP, m);
    print("RECEIVED STOP");

    exit(0);
}

void handle_sigint(){
    stop();
    eprint("exiting SIGINT");
    exit(0);
}

void got_stop(message* m){
    print("GOT STOP")

    message to_send = {.mtype = STOP, .sender_id = client_id};
    send_message(server_queue, to_send);
    eprint("exiting GOT STOP");
    exit(0);
}

void init(){
    message m = {.mtype = INIT, .queue = client_queue};
    send_message(server_queue, m);
    print("SENDING INIT");

    receive_message(client_queue, INIT, m);
    print("RECEIVED MESSAGE")
    client_id = m.client_id;
    print("CLIENT ID: %d", client_id);
}

void connect(int connect_to_id){
    message m = {.mtype = CONNECT, .sender_id = client_id, .client_id = connect_to_id};
    send_message(server_queue, m);
    print("SENDED CONNECT WITH %d", connect_to_id);

    chat_mode = true;
}

void got_connect(message *m){
    second_client_queue = m->queue;

    print("SECOND CLIENT QUEUE: %d", second_client_queue);
    chat_mode = true;
}

void disconnect(){
    if (not chat_mode){
        print("U ARE NOT CONNECTED LOL")
    }
    message m = {.mtype = DISCONNECT, .sender_id = client_id};
    send_message(server_queue, m);
    chat_mode = false;


    send_message(second_client_queue, m);
}

void got_disconnect(message *m){
    print("GOT DISCONNECT")
    if (not chat_mode){
        print("U ARE NOT CONNECTED LOL")
    }

    chat_mode = false;
    print("DISCONNECTING");
    message m2 = {.mtype = DISCONNECT, .sender_id = client_id};
    send_message(server_queue, m2);
}

void list(){
    message m = {.mtype = LIST};
    send_message(server_queue, m);
}

void got_chat(message *m){
    print("GOT NEW MESSAGE :O");
    print("MESSAGE FROM %d", m->sender_id);
    print("MESSAGE TEXT: %s", m->message);
}

void send_chat(){
    if (not chat_mode){
        print("U ARE NOT CONNECTED!!!!!!!!!")
        return;
    }
    print("SENDING NEW MESSAGE");
    print("ENTER MESSAGE");
    message m = {.mtype = CHAT, .sender_id = client_id};

    fgets(m.message, MAX_MESSAGE_LEN, stdin);
    send_message(second_client_queue, m);
    print("MESSAGE SENT");
}

int read_from_user(int*);

int main(void){
    signal(SIGINT, handle_sigint);

    atexit(close_client_queue);

    server_queue = create_queue(HOME, ID, 0);
    print("server queue %d", server_queue);

    client_queue = create_queue(HOME, CLIENT_ID, IPC_CREAT | IPC_EXCL |  0666);
    print("client queue %d", client_queue);
    if(client_queue == -1){
        exit(-1);
    }

    init();

    message received;
    int connect_to_id;

    while (true){
        switch (read_from_user(&connect_to_id)) {
            case STOP:
                stop();
                break;
            case LIST:
                list();
                break;
            case CONNECT:
                connect(connect_to_id);
                break;
            case DISCONNECT:
                disconnect();
                break;
            case CHAT:
                send_chat();
                break;
            default:
                break;
        }

        receive_message_NOWAIT(client_queue, -ALL_TYPES, received);

        switch (received.mtype) {
            case STOP:
                got_stop(&received);
                break;

            case CONNECT:
                got_connect(&received);
                break;

            case DISCONNECT:
                got_disconnect(&received);
                break;
            case CHAT:
                got_chat(&received);
                break;

            default:
                continue;
        }
    }

    return 0;
}

int read_from_user(int *id){
    char buf[5];

    initscr();
    noecho();
    cbreak();
    timeout(300);

    int to_return =-1;
    int c = getch();
    switch (c) {
        case 'l': print("list");
            to_return = LIST;
            break;
        case 'c': print("\nPodaj id");

            nocbreak();
            echo();
            endwin();
            if (fgets(buf, sizeof(buf), stdin) != NULL) {
                if (strlen(buf) <= 1) {
                    print("No number")
                    break;
                }
                *id = get_first_int(buf);
                if (*id == INT_MAX) {
                    print("sth is wrong with entered number");
                    break;
                }
                print("\nID to: %d", *id);
            }
            return CONNECT;
        case 'd': print("\ndisconnect");
            to_return = DISCONNECT;
            break;
        case 's': print("\nstop");
            to_return = STOP;
            break;
        case 'n': print("\nsend new message");
            to_return = CHAT;
            break;
        default:
            break;
    }

    nocbreak();
    echo();
    endwin();

    return to_return;
}
#pragma clang diagnostic pop