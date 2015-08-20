#include "apue.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>


void print_ids();


int main(int argc, char *argv[]) {
    if (argc != 2) {
        err_quit("Usage: %s <target uid>", argv[0]);
    }

    long target_uid = parse_long(argv[1]);
    if (target_uid < 0) {
        err_quit("Target UID should be non-negative");
    }

    print_ids();
    printf("\nCalling setuid(%ld)\n\n", target_uid);
    sys_chk(setuid(target_uid));
    print_ids();

    return EXIT_SUCCESS;
}
