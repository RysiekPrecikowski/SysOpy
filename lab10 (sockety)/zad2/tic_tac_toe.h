//
// Created by rysiek on 25/05/2021.
//

#ifndef SYSOPY_TIC_TAC_TOE_H
#define SYSOPY_TIC_TAC_TOE_H

#include "utils.h"
#include <sys/poll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#define BLANK '-'


#define message_format "%c %s %d %d %d %d"
#define message_size (50)


#define login_format "login %s"
#define wrong_name_format "name %s is taken!!!"

#define exit_message "let me outtttt!!"
#define ping_message "ping :o"

#define move_message_format "%s %s"
#define good_name_message "good :)))))"

char number_to_symbol(int n){
    switch (n) {
        case 0:
            return 'O';
        case 1:
            return 'X';
        default:
            return '\0';
    }
}

#define prepare_board(board) {set_array(board, 9, BLANK); board[9] = '\0';}

typedef char symbol;

typedef struct {
    int my_turn;
    int end;
    int you_won;
    int retry;
    int draw;

    int counter;
}game_info;

void read_message(char* message, game_info* info_out){
    symbol s;
    char board[9];
    game_info info;

    sscanf(message, message_format, &s, board, &info.my_turn, &info.end, &info.you_won, &info.draw);

    print("YOUR SYMBOL: %c", s);

    if (info.end){
        if (info.draw){
            printf("ITS A DRAW !!!\n");
        } else if(info.you_won){
            printf("U WON !!!!\n");
        } else {
            printf("YOU LOST !!!\n");
        }

    }else {
        if (info.my_turn) {
            printf("Your turn, choose number from 0 to 8\n");
        } else {
            printf("Your opponent turn\n");
        }
    }

    printf("BOARD:");
    for (int i = 0; i < 9; i+=3) {
        printf("\n    %d %d %d", i, i + 1, i + 2);
        printf("   %c %c %c", board[i], board[i + 1], board[i + 2]);
    }
    printf("\n");
    fflush(stdout);

    if (info.end and info.my_turn)
        exit(0);

    info_out->my_turn = info.my_turn;
}

char* prepare_message(symbol s, char* board, game_info* info){
    char* res = calloc(message_size, sizeof (char ));
    sprintf(res, message_format, s, board, info->my_turn, info->end, info->you_won, info->draw);
    return res;
}

int to_1d(int i, int j){
    return i * 3 + j;
}

bool check_draw(char board[10]){
    for_i(9){
        if(board[i] == BLANK)
            return false;
    }
    return true;
}

void check_win(symbol s, char board[10], game_info* info0, game_info* info1){
    bool win = false;

    for(int i = 0 ; i < 3 ; i++){
        int counter =0;
        for (int j = 0 ; j < 3 ; j ++){
            if (board[to_1d(i, j)] == s)
                counter++;
        }
        if (counter == 3){
            win = true;
            break;
        }

        counter =0;
        for (int j = 0 ; j < 3 ; j ++){
            if (board[to_1d(j, i)] == s)
                counter++;
        }
        if (counter == 3){
            win = true;
            break;
        }
    }

    int counterl = 0, counterr = 0;
    for (int i = 0 ; i < 3 ; i++) {
        if (board[to_1d(i, i)] == s)
            counterl++;
        if (board[to_1d(i, 3 - 1 - i)] == s)
            counterr++;
    }
//    print("%d %d", counterl, counterr);
    if (counterl == 3 or counterr == 3 or win){
        info0->end=true;
        info0->you_won= true;

        info1->end= true;
        info1->you_won=false;
    }
}

bool check_board(int pos, char board[10]){
    if (pos > 8 or pos < 0){
        return false;
    }

    if (board[pos] != BLANK){
        return false;
    }

    return true;
}



void server_got_position(char* position, int player0,
                         char boards[][10],
                         symbol* client_symbols,
                         int* clients_games,
                         int* client_fds, struct sockaddr *adrr){


    int pos = string_to_int(position);

    print("pos %d", pos);

    if (check_board(pos, boards[player0]) == false){
        game_info info = {.my_turn = true, .retry = true};
        char* message = prepare_message(client_symbols[player0], boards[player0], &info);
//        send(fds[player0].fd,message, strlen(message), 0);

        if (sendto(client_fds[player0], message, strlen(message), 0, &adrr[player0], sizeof (adrr[player0])) < 0){
            perror("SEND to 0");
        }
        printf("WRONG!!!!!!!!!!!!\n");
        return;
    }

    symbol s = client_symbols[player0];
    int player1 = clients_games[player0];


    boards[player0][pos] = s;
    boards[player1][pos] = s;


    game_info info0 = {.my_turn = false, .end = false};
    game_info info1 = {.my_turn = true, .end = false};

    if (check_draw(boards[player0])){
        info0.draw = true;
        info1.draw = true;

        info0.end = true;
        info1.end = true;

    } else {
        check_win(s, boards[player0], &info0, &info1);
    }



    char* message_to0 = prepare_message(client_symbols[player0], boards[player0], &info0);
    char* message_to1 = prepare_message(client_symbols[player1], boards[player1], &info1);



    if (sendto(client_fds[player0], message_to0, strlen(message_to0), 0, &adrr[player0], sizeof (adrr[player0])) < 0){
        perror("SEND to 0");
    }
//    sleep_seconds(0.1);
    if (sendto(client_fds[player1], message_to1, strlen(message_to1), 0, &adrr[player1], sizeof (adrr[player1])) < 0){
        perror("SEND to 1");
    }

    free(message_to0);
    free(message_to1);
}





void start_game(int player0, int player1,
                char boards[][10],
                symbol* client_symbols,
                int* clients_games,
                int* client_fds, struct sockaddr adrr0, struct sockaddr addr1){

    print("STARTING GAME %d and %d", player0, player1);

    prepare_board(boards[player0]);
    prepare_board(boards[player1]);

    clients_games[player1] = player0;
    clients_games[player0] = player1;

    srand(time(NULL));
    int r = rand() % 2;

    client_symbols[player0] = number_to_symbol(r);
    client_symbols[player1] = number_to_symbol(1 - r);

    r = rand() % 2;
    game_info info0 = {.my_turn = r, .end = false};
    game_info info1 = {.my_turn = 1-r, .end = false};

    char* message_to0 = prepare_message(client_symbols[player0], boards[player0], &info0);
    char* message_to1 = prepare_message(client_symbols[player1], boards[player1], &info1);


    if (sendto(client_fds[player0], message_to0, strlen(message_to0), 0, &adrr0, sizeof (adrr0)) < 0){
        perror("SEND to 0");
    }
//    sleep_seconds(0.1);
    if (sendto(client_fds[player1], message_to1, strlen(message_to1), 0, &addr1, sizeof (addr1)) < 0){
        perror("SEND to 1");
    }

    free(message_to0);
    free(message_to1);
}

#endif //SYSOPY_TIC_TAC_TOE_H
