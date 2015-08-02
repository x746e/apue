/**
 * Function to print various information about a file descriptor.
 */
#include "apue.h"
#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

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


void check_fd(int);
void guess_file_name(int fd, char *buf, int buflen);


int
main() {
    check_fd(0);
    check_fd(1);
    check_fd(2);
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

    char guessed_file_name[PATH_MAX];
    guess_file_name(fd, guessed_file_name, sizeof guessed_file_name);

    // TODO: Move it to linux-specific file.
    /*
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
    */
    printf("Guessed file name: %s\n", guessed_file_name);
}
