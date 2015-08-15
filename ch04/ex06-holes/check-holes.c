// Needed for SEEK_HOLE and SEEK_DATA
#define _GNU_SOURCE         // Linux
#define __BSD_VISIBLE 1     // FreeBSD, DragonFlyBSD
#define __EXTENSIONS__      // Solaris

#include "apue.h"
#include "common.h"
#include <fcntl.h>
#include <errno.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        err_quit("Usage: %s <file-to-check>", argv[0]);
    }

    int fd;
    sys_chk(fd = open(argv[1], O_RDONLY));

// Defined and works on FreeBSD (10.1).
// Defined and works on Solaris (openindiana 5.11).
// Defined on DragonFly (4.2).  Doesn't work, returns EINVAL.  Though lseek(2)
// and pathconf(2) doesn't mention holes at all.
// Not defined on Linux.
#ifdef _PC_MIN_HOLE_SIZE
    long min_hole_size;
    errno = 0;
    if ((min_hole_size = fpathconf(fd, _PC_MIN_HOLE_SIZE)) == -1) {
        if (errno == 0) {
            printf("MIN_HOLE_SIZE: no limit\n");
        } else {
            perror("fpathconf(fd, _PC_MIN_HOLE_SIZE)");
        }
    }
    printf("MIN_HOLE_SIZE: %ld\n", min_hole_size);
#endif

    struct stat stat;
    sys_chk(fstat(fd, &stat));
    printf("Block size: %lld\n", (long long)stat.st_blksize);

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
