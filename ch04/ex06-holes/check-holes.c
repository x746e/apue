// Needed for SEEK_HOLE and SEEK_DATA
#define _GNU_SOURCE         // Linux
#define __BSD_VISIBLE 1     // FreeBSD
#define __EXTENSIONS__      // Solaris

#include "apue.h"
#include "common.h"
#include <fcntl.h>


int main(int argc, char *argv[]) {
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
            printf("Hole from %lld to %lld\n",
                    (long long)hole_start, (long long)hole_end);
        }
    }
}
