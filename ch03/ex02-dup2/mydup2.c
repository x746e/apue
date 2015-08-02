#include "apue.h"
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <errno.h>
#include <unistd.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define yellow(arg) ANSI_COLOR_YELLOW arg ANSI_COLOR_RESET
#define red(arg) ANSI_COLOR_RED arg ANSI_COLOR_RESET
#define green(arg) ANSI_COLOR_GREEN arg ANSI_COLOR_RESET

void check_fd(int fd);
int mydup2(int oldfd, int newfd);


int
main() {
    check_fd(0);
    check_fd(1);
    check_fd(2);
}

void
dup2_test_main(int argc, char *argv[]) {
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


void
check_fd(int fd) {
    printf(yellow(">> Checking ") green("fd#%d") "\n", fd);

    off_t current_position;
    if ((current_position = lseek(fd, 0, SEEK_CUR)) == -1) {
        err_ret("Can't get current position");
    } else {
        printf("current_position: %zd\n", current_position);
    }

    int descriptor_flags;
    if ((descriptor_flags = fcntl(fd, F_GETFD)) < 0) {
        err_sys("Can't get descriptor flags");
    }
    printf("FD_CLOEXEC: %d\n", descriptor_flags & FD_CLOEXEC);

    int file_status_flags;
    if ((file_status_flags = fcntl(fd, F_GETFL)) < 0) {
        err_sys("Can't get file status flags");
    }
    printf("File status: ");
    switch (file_status_flags & O_ACCMODE) {
        case O_RDONLY:
            printf("O_RDONLY");
            break;
        case O_WRONLY:
            printf("O_WRONLY");
            break;
        case O_RDWR:
            printf("O_RDWR");
            break;
        default:
            printf("Unknown access mode");
    }
    if (file_status_flags & O_APPEND) {
        printf(", O_APPEND");
    }
    if (file_status_flags & O_NONBLOCK) {
        printf(", O_NONBLOCK");
    }
    if (file_status_flags & O_SYNC) {
        printf(", O_SYNC");
    }
#ifdef O_FSYNC
    if (file_status_flags & O_FSYNC) {
        printf(", O_FSYNC");
    }
#endif
    printf("\n");

    char link_target[PATH_MAX] = { 0 };
    int r;
    #define prefix "/proc/self/fd/"
    // Figure out chars needed for the fd number string representation.
    int chars;
    if (fd == 0) {
        chars = 1;
    } else {
        chars = (int)floor(log10(fd));
    }
    size_t link_size = (sizeof prefix + chars + 1) * sizeof(char);
    char *link;
    if ((link = malloc(link_size)) == NULL) {
        err_sys("malloc error for allocating link");
    }
    sprintf(link, prefix "%d", fd);
    if ((r = readlink(link, link_target, PATH_MAX)) == -1) {
        err_sys("readlink failure");
    } else if (r > PATH_MAX - 1) {
        err_quit("Cannot read link: link's size > PATH_MAX");
    }
    free(link);
    printf("Guessed file name: %s\n", link_target);
}
