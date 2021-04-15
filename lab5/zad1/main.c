#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>

#define and &&

#define for_i(n) for(int i = 0 ; i < n ; i++)
#define for___(n, i) for(i = 0 ; i < n ; i++)

#define COMMAND_START " = "
#define COMMAND "command"
#define PIPE "|"

#define MAX_ARGC 10
#define MAX_COMMANDS 10

typedef struct {
    char *program;
    char *args[MAX_ARGC];
    int argc;
} small_command;

typedef struct {
    small_command *commands[MAX_COMMANDS];
    int n;
} command;

typedef struct {
    command *commands[MAX_COMMANDS];
    int n;
} commands_container;

void print_small_command(small_command *command) {
    printf("%s ", command->program);

    for_i(command->argc) {
        printf("%s ", command->args[i]);
    }
}

void print_command(command *comm) {
    for_i(comm->n) {
        print_small_command(comm->commands[i]);
        printf(" | ");
    }
}

void print_commands_container(commands_container *container) {
    printf("COMMANDS:\n");
    for_i(container->n) {
        printf("%d ", i);
        print_command(container->commands[i]);
        printf("\n");
    }
    printf("\n");
}

int get_command_number(char *line) {
    int number;
    sscanf(line, "%*[^0123456789]%d", &number);
    return number;
}

char *copy_string(char *str) {
    char *res = calloc(strlen(str) + 1, sizeof(char));
    strncpy(res, str, strlen(str));
    return res;
}

small_command *read_small_command(char *text) {
    small_command *command = malloc(sizeof(small_command));
    char *pch;
    pch = strtok(text, " ");

    command->program = copy_string(pch);
    command->args[0] = command->program;
    command->argc = 1;

    pch = strtok(NULL, " ");
    while (pch != NULL) {
        command->args[command->argc] = copy_string(pch);
        command->argc += 1;
        pch = strtok(NULL, " ");
    }
    command->args[command->argc] = NULL;
    return command;
}

command *read_command(char *line) {
    char *start = strstr(line, COMMAND_START);
    if (start != NULL)
        start += strlen(COMMAND_START);

    command *res = malloc(sizeof(command));
    res->n = 0;

    char *pch;
    char *rest = line;
    pch = strtok_r(start, PIPE, &rest);

    while (pch != NULL) {
        if (pch[strlen(pch) - 1] == ' ' || pch[strlen(pch) - 1] == '\n')
            pch[strlen(pch) - 1] = '\0';

        small_command *smallCommand = read_small_command(pch);
        res->commands[res->n] = smallCommand;
        res->n += 1;

        pch = strtok_r(NULL, PIPE, &rest);
        if (pch != NULL)
            pch += 1;
    }
    return res;
}

commands_container *prepare_commands(FILE *fp) {
    commands_container *res = malloc(sizeof(commands_container));
    res->n = 0;

    char *line = NULL;
    size_t len = 0;

    while (read = getline(&line, &len, fp) > 1) {
        res->commands[res->n] = read_command(line);
        res->n += 1;
    }
    return res;
}

// 0 -> odczyt 1 -> zapis
void pipe_start(int out[2], int in[2]) {
    close(out[0]);
//    close(in[1]);
//    close(in[0]);
    dup2(out[1], STDOUT_FILENO);
}

void pipe_mid(int out[2], int in[2]) {
    close(in[1]);
    close(out[0]);
    dup2(out[1], STDOUT_FILENO);
    dup2(in[0], STDIN_FILENO);
}

void pipe_end(int out[2], int in[2]) {
    close(out[0]);
    close(out[1]);
    close(in[1]);
    dup2(in[0], STDIN_FILENO);
}

void swap_pipes(int out[2], int in[2]) {
    close(in[1]);
    in[0] = out[0];
    in[1] = out[1];
    pipe(out);
}

void execute_one_command(commands_container *com) {
    print_commands_container(com);
    int j, i;
    int out[2], in[2];
    pipe(out);

    for___(com->n, i) {
        command *curr_command = com->commands[i];

        for___(curr_command->n, j) {
            small_command *curr = curr_command->commands[j];
            pid_t child = fork();

            if (child == 0) {
                if (i == com->n - 1 and j == curr_command->n - 1) { //last command
                    pipe_end(out, in);
                } else if (i == 0 and j == 0) { //first command
                    pipe_start(out, in);
                } else {
                    pipe_mid(out, in);
                }

                execvp(curr->program, curr->args);
            }

            swap_pipes(out, in);

            waitpid(child, NULL, 0);
        }
    }
    printf("\n");
}

void execute_commands(FILE *fp, commands_container *commands) {
    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, fp) > 1) {
        commands_container current;
        current.n = 0;

        char *pch;
        pch = strtok(line, " |");
        while (pch != NULL) {
            int i = get_command_number(pch);
            current.commands[current.n] = commands->commands[i];
            current.n += 1;
            pch = strtok(NULL, " |");
        }

        execute_one_command(&current);
    }
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        return -1;
    }
    FILE *file = fopen(argv[1], "r");

    commands_container *commandsContainer = prepare_commands(file);
    print_commands_container(commandsContainer);

    execute_commands(file, commandsContainer);

    fclose(file);
    return 0;
}