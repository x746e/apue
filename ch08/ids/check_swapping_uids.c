/**
 * Check swapping effective user ID between real user id and saved-set-user-id.
 */
#include "apue.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


void print_ids();


int main(int argc, char *argv[]) {
    print_ids();
    long saved_uid = geteuid();
    printf("\nSaved euid: %ld\n", saved_uid);

    printf("\nCalling seteuid(getuid())\n");
    sys_chk(seteuid(getuid()));
    print_ids();

    printf("\nCalling seteuid(saved_uid)\n");
    sys_chk(seteuid(saved_uid));
    print_ids();

    return EXIT_SUCCESS;
}
