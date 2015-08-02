#define _BSD_SOURCE
#include "apue.h"
#include <dirent.h>
#include <errno.h>

void main(int argc, char *argv[]) {
    if (argc != 2) {
        err_quit("Usage: %s <directory>", argv[0]);
    }

    DIR *dir;
    if ((dir = opendir(argv[1])) == NULL) {
        err_sys("Can't open directory `%s`", argv[1]);
    }

    struct dirent *ent;

    while (1) {
        errno = 0;
        ent = readdir(dir);
        if (ent == NULL && errno != 0) {
            err_sys("readdir error");
        } else if (ent == NULL) {
            break;
        } else {
            printf("%zd\t%s", ent->d_ino, ent->d_name);
            switch (ent->d_type) {
                case DT_BLK:
                    printf("\tblock device");
                    break;
                case DT_CHR:
                    printf("\tcharacter device");
                    break;
                case DT_DIR:
                    printf("\tdirectory");
                    break;
                case DT_FIFO:
                    printf("\tnamed pipe");
                    break;
                case DT_LNK:
                    printf("\tsymbolic link");
                    break;
                case DT_REG:
                    printf("\tregular file");
                    break;
                case DT_SOCK:
                    printf("\tUNIX socket");
                    break;
            }
            printf("\n");
        }
    }

    if (closedir(dir) == -1) {
        err_sys("Error while closing directory");
    }
}
