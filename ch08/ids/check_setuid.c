#include "apue.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


void print_ids();


void usage(char *program_name) {
    err_quit("Usage: %s [-e] <target uid>", program_name);
}


int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        usage(argv[0]);
    }
    int uid_arg_idx = 1;
    bool use_seteuid = false;
    if (argc == 3) {
        if (strcmp("-e", argv[1]) == 0) {
            uid_arg_idx = 2;
            use_seteuid = true;
        } else {
            usage(argv[0]);
        }
    }

    long target_uid = parse_long(argv[uid_arg_idx]);
    if (target_uid < 0) {
        err_quit("Target UID should be non-negative");
    }

    print_ids();
    printf("\nCalling %s(%ld)\n\n",
           use_seteuid ? "seteuid" : "setuid",
           target_uid);
    if (use_seteuid) {
        sys_chk(seteuid(target_uid));
    } else {
        sys_chk(setuid(target_uid));
    }
    print_ids();

    return EXIT_SUCCESS;
}
