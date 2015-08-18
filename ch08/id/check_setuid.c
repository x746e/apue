#include "apue.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


void print_ids();


int main(int argc, char *argv[]) {
    long target_uid;
    if (argc > 2) {
        err_quit("Usage: %s [target uid]", argv[0]);
    } else if (argc == 2) {
        errno = 0;
        target_uid = strtol(argv[1], NULL, 10);
        if (errno != 0) {
            err_sys("Cannot convert string to number: %s", argv[1]);
        }
        if (target_uid < 0) {
            err_quit("Target UID should be non-negative");
        }
    } else {
        target_uid = 1;  // user "daemon".
    }

    printf("Calling setuid(%ld)\n", target_uid);
    sys_chk(setuid(target_uid));
    print_ids();

    return EXIT_SUCCESS;
}
