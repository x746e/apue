#include "apue.h"


int main() {
    printf("Size of char: %ld\n", sizeof(char));
    printf("Size of short: %ld\n", sizeof(short int));
    printf("Size of int: %ld\n", sizeof(int));

    union {
        short s;
        unsigned char c[2];
    } u;

    u.s = 0x0102;

    if (u.c[0] == 2 && u.c[1] == 1) {
        printf("Little endian\n");
    } else if (u.c[0] == 1 && u.c[1] == 2) {
        printf("Big endian\n");
    } else {
        printf("WTF: c[0]: %d, c[1]: %d?\n", u.c[0], u.c[1]);
    }

    return EXIT_SUCCESS;
}
