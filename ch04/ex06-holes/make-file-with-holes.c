#include "apue.h"
#include "common.h"
#include <unistd.h>
#include <fcntl.h>


// Use max block size across Linux/FreeBSD/Sunos (from Sunos).
#define HOLE_SIZE 131072

int main (int argc, char *argv[]) {
    if (argc != 2) {
        err_quit("Usage: %s <file-to-create>", argv[0]);
    }

    int fd;
    sys_chk(fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, FILE_MODE));

    int num_holes = 2;
    int i;
    char *buf;
    sys_ptr_chk(buf = malloc(HOLE_SIZE));
    memset(buf, 'z', HOLE_SIZE);

    for (i = 0; i < num_holes; i++) {
        sys_chk(lseek(fd, HOLE_SIZE, SEEK_CUR));
        sys_chk(write(fd, buf, HOLE_SIZE));
    }

    sys_chk(close(fd));
}


