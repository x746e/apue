#include "apue.h"
#include "common.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


void main (int argc, char *argv[]) {
    if (argc != 2) {
        err_quit("Usage: %s <file-to-create>", argv[0]);
    }

    int fd;
    sys_chk(fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, MODE_666));

    char c;
    for (c = 'a'; c <= 'c'; ++c) {
        sys_chk(write(fd, &c, 1));
        sys_chk(write(fd, "\n", 1));
        sys_chk(lseek(fd, 2*4096, SEEK_CUR));
    }

    sys_chk(close(fd));
}
