#include "apue.h"
#include "common.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


long get_hole_size(int fd);


int main (int argc, char *argv[]) {
    if (argc != 2) {
        err_quit("Usage: %s <file-to-create>", argv[0]);
    }

    int fd;
    sys_chk(fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, FILE_MODE));

    long hole_size = get_hole_size(fd);
    char c;
    for (c = 'a'; c <= 'c'; ++c) {
        sys_chk(write(fd, &c, 1));
        sys_chk(write(fd, "\n", 1));
        sys_chk(lseek(fd, 2 * hole_size, SEEK_CUR));
    }

    sys_chk(close(fd));
}


/**
 * Figure out hole size.
 *
 * Return max from block size and pathconf(fd, _PC_MIN_HOLE_SIZE) (if supported
 * on the platform).
 */
long get_hole_size(int fd) {
    long min_hole_size = 0;
    blksize_t block_size;
#ifdef _PC_MIN_HOLE_SIZE
    if ((min_hole_size = fpathconf(fd, _PC_MIN_HOLE_SIZE)) == -1) {
        // fpathconf error or no limit.
    }
#endif
    struct stat stat;
    sys_chk(fstat(fd, &stat));
    block_size = stat.st_blksize;

    return max(block_size, min_hole_size);
}
