#include "apue.h"
#include <stdio.h>
#include <stdlib.h>


void greeter() {
    printf("hello\n");
}

typedef void (*Exitfunc)(void);

int my_atexit(Exitfunc func) {
    return atexit(func);
}

void main() {
    if (my_atexit(greeter) != 0) {
        err_sys("atexit");
    };
}
