#define _XOPEN_SOURCE 1
#define _XOPEN_SOURCE_EXTENDED 1
#include <stdio.h>
#include <sys/types.h>
#include <ftw.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <dirent.h>

static int num_reg;
static int num_dir;

int process (const char *path, const struct stat *stat_buf, int type, struct FTW *ftwp) {
/*Zlicz pliki regularne i katalogi, zwiększając zmienne num_reg i num_dir.
Jeśli nie da się otworzyć katalogu zwróć błąd na stdout wraz z wypisaniem tego katalogu */


    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    struct stat stat;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
            lstat(dir->d_name, &stat);
            if (S_ISDIR(stat.st_mode)){
                num_dir ++;
            } else if (S_ISREG(stat.st_mode)){
                num_reg ++;
            }
        }
        closedir(d);
    }

/* koniec */
    return (0);
}

int main (int argc, char *argv[]) {
    int i;
    for (i = 1; i < argc; i++) {
        num_reg = num_dir = 0;
        nftw(argv[i], process, FOPEN_MAX, FTW_PHYS);
        printf("Totals for %s:\n", argv[i]);
        printf("  Regular files: %d\n", num_reg);
        printf("  Directories: %d\n", num_dir);
    }
    return (0);
}
