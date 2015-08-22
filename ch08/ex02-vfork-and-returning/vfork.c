#define _BSD_SOURCE
#define __BSD_VISIBLE 1
#include <unistd.h>
#include "apue.h"
#include <stdlib.h>
#include <stdio.h>


void func() {
    pid_t pid;
    printf("%d: beginning of the func()\n", getpid());

    if ((pid = vfork()) < 0) {
        err_sys("vfork error");
    } else if (pid == 0) {  // child
        printf("%d: after vfork, child\n", getpid());
        return;
        //_exit(0);
    } else {  // parent
        printf("%d: after vfork, parent\n", getpid());
        return;
    }

    printf("%d: shouldn't get there\n", getpid());
}


int main() {
    printf("%d: In main, before func()\n", getpid());
    func();
    printf("%d: In main, after func()\n", getpid());
    return EXIT_SUCCESS;
}
