#include "apue.h"
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>


extern void check_fd(int fd);
int mydup2(int oldfd, int newfd);


int
main(int argc, char *argv[]) {
    int fd1, fd2;
    if (argc != 2) {
        err_quit("Usage: %s file", argv[0]);
    }
    if ((fd1 = open(argv[1], O_RDONLY)) < 0) {
        err_sys("Can't open file `%s`", argv[1]);
    }

    fd2 = mydup2(fd1, 42);

    lseek(fd1, 0, SEEK_END);

    check_fd(fd1);
    check_fd(fd2);
}


int mydup2(int oldfd, int newfd) {
    struct stat s;

    // Check if oldfd is a valid descriptor, fail with EBADF if it's not.
    if (fstat(oldfd, &s) < 0) {
        errno = EBADF;
        return -1;
    }
    // Check newfd is in valid range, fail with EBADF otherwise.
    int openmax = sysconf(_SC_OPEN_MAX);  // Can return -1; that means either no limit, or error getting the limit.  Either case is fine here.
    if (newfd < 0 || newfd > openmax) {
        errno = EBADF;
        return -1;
    }
    // If oldfd == newfd just return newfd.
    if (oldfd == newfd) {
        return newfd;
    }
    // If newfd is open, close it.
    if (fstat(newfd, &s) == 0) {
        close(newfd);
    }

    int to_close_ptr = 0;
    int *to_close = malloc(sizeof(int) * newfd);
    int current_try;
    int i;
    while (1) {
        if ((current_try = dup(oldfd)) < 0) {
            err_sys("dup failed");
        }

        if (current_try == newfd) {
            for (i = 0; i < to_close_ptr; ++i) close(to_close[i]);
            free(to_close);
            return newfd;
        } else {
            to_close[to_close_ptr++] = current_try;
        }
    }
}
