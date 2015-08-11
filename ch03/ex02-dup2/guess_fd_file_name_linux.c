#include "apue.h"
#include <limits.h>
#include <unistd.h>
#include <stdio.h>


void
guess_file_name(int fd, char *buf, int buflen) {

    char self_fd_link[PATH_MAX] = { 0 };
    int r;

    #define prefix "/proc/self/fd/"
    sprintf(self_fd_link, prefix "%d", fd);

    if ((r = readlink(self_fd_link, buf, buflen)) == -1) {
        err_sys("readlink failure");
    } else if (r > buflen - 1) {
        err_quit("Cannot read link: link's size > PATH_MAX");
    }

}
