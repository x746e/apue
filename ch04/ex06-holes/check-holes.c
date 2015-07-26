#define _GNU_SOURCE
#include "apue.h"
#include "common.h"
#include <unistd.h>
#include <fcntl.h>


void main(int argc, char *argv[]) {
    if (argc != 2) {
        err_quit("Usage: %s <file-to-check>", argv[0]);
    }

    int fd;
    sys_chk(fd = open(argv[1], O_RDONLY));

    off_t file_end = lseek(fd, 0, SEEK_END);
    off_t current_offset, hole_start, hole_end;

    current_offset = 0;

    while (current_offset < file_end) {
        sys_chk(current_offset = hole_start
                        = lseek(fd, current_offset, SEEK_HOLE));
        if (hole_start < file_end) {
            sys_chk(current_offset = hole_end
                        = lseek(fd, hole_start, SEEK_DATA));
            printf("Hole from %zd to %zd\n", hole_start, hole_end);
        }
    }
}
