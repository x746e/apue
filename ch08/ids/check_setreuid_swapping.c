/**
 * Check swapping effective user ID with real user id with setreuid function.
 */
#include "apue.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>


void print_ids();


int main(int argc, char *argv[]) {
    print_ids();

    printf("\nCalling setreuid(geteuid(), getuid())\n");
    sys_chk(setreuid(geteuid(), getuid()));
    print_ids();

    printf("\nCalling setreuid(geteuid(), getuid()) again\n");
    sys_chk(setreuid(geteuid(), getuid()));
    print_ids();

    return EXIT_SUCCESS;
}
