#include "apue.h"
#include <stdio.h>
#include <sys/utsname.h>


int main(int argc, char *argv[]) {
    struct utsname buf;
    if (uname(&buf) == -1) {
        err_sys("uname");
    }
    printf("sysname: %s\n", buf.sysname);
    printf("nodename: %s\n", buf.nodename);
    printf("release: %s\n", buf.release);
    printf("version: %s\n", buf.version);
    printf("machine: %s\n", buf.machine);
    return 0;
}
