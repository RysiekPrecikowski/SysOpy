#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>


int isDirDot(char *dirName){
    if (strlen(dirName) > 2)
        return 0;
    return strcmp(".", dirName) == 0 || strcmp("..", dirName) == 0;
}

int main(int argc, char* argv[]) {
    DIR *katalog;
    if (argc != 2) {
        printf("Wywołanie %s ścieżka", argv[0]);
        return 1;
    }
    struct dirent *dir;



/*Otwórz katalog, w przypadku błędu otwarcia zwróć błąd funkcji otwierającej i zwrócć 1. 
Wyświetl zawartość katalogu katalog, pomijając "." i ".."
Jeśli podczas tej operacji wartość errno zostanie ustawiona, zwróć błąd funkcji czytającej oraz wartość 2. */


    DIR *d = opendir(argv[1]);

    if (!d){
        printf("ERROR IN OPENING DIR %d", errno);
        return 1;
    }

    while ((dir = readdir(d)) != NULL) {
        if(isDirDot(dir->d_name) == 1)
            continue;

        printf("%s\n", dir->d_name);
        if (errno != 0){
            printf("ERROR %d\n", errno);
            return 2;
        }
    }






/*koniec*/
    closedir(katalog);
    return (0);
}
