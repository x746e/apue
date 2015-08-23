#define _BSD_SOURCE
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include "common.h"

int main() {
    int opt;

    DIR *dir;
    sys_ptr_chk(dir = opendir("/"));

    sys_chk(opt = fcntl(dirfd(dir), F_GETFD));
    printf("opendir(\"/\"): FD_CLOEXEC is %s\n", (opt & FD_CLOEXEC) ? "set" : "not set");
    sys_chk(closedir(dir));

    int fd = open("/", O_RDONLY);
    sys_chk(opt = fcntl(fd, F_GETFD));
    printf("open(\"/\"): FD_CLOEXEC is %s\n", (opt & FD_CLOEXEC) ? "set" : "not set");

    return EXIT_SUCCESS;
}
