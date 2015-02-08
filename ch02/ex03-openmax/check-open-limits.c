#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>


int main() {
    printf("sysconf(_SC_OPEN_MAX): %ld\n", sysconf(_SC_OPEN_MAX));
    printf("OPEN_MAX: %d\n", OPEN_MAX);
    exit(EXIT_SUCCESS);
}
