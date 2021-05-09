#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

#define READ 0
#define WRITE 1

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Not a suitable number of program parameters\n");
        exit(1);
    }

    int toChildFD[2];
    int toParentFD[2];

    pipe(toChildFD);
    pipe(toParentFD);


    int val1, val2, val3 = 0;

    pid_t pid;

    if ((pid = fork()) == 0) {
        close(toChildFD[WRITE]);
        close(toParentFD[READ]);
        //odczytaj z potoku nienazwanego wartosc przekazana przez proces macierzysty i zapisz w zmiennej val2
        char buf[10];
        memset(buf, '\0', 10);
        read(toChildFD[READ], buf, sizeof(buf));

        val2 = atoi(buf);


        val2 = val2 * val2;

        sprintf(buf, "%d", val2);

        //wyslij potokiem nienazwanym val2 do procesu macierzysego
        write(toParentFD[WRITE], buf, strlen(buf));

    } else {
        close(toChildFD[READ]);
        close(toParentFD[WRITE]);
        val1 = atoi(argv[1]);
        //wyslij val1 potokiem nienazwanym do priocesu potomnego
        char buf[10];
        sprintf(buf, "%d", val1);
        write(toChildFD[WRITE], buf, strlen(buf));

        sleep(1);

        //odczytaj z potoku nienazwanego wartosc przekazana przez proces potomny i zapisz w zmiennej val3

        read(toParentFD[READ], buf, sizeof (buf));
        val3 = atoi(buf);
        printf("%d square is: %d\n", val1, val3);
    }
    return 0;
}
