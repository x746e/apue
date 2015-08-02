#include "apue.h"
#include <fcntl.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        err_quit("Usage: %s <directory>", argv[0]);
    }

    int fd;
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        err_sys("Can't open `%s`", argv[1]);
    }

    // Check it's a dir.
    struct stat st;
    if (fstat(fd, &st) == -1) {
        err_sys("Can't stat");
    }

    if (!S_ISDIR(st.st_mode)) {
        err_quit("Argument is not a directory");
    }

    char buff[BUFSIZ] = { 0 };
    int n;
    if ((n = read(fd, buff, BUFSIZ)) == -1) {
        err_sys("Can't read");
    } else {
        write(STDOUT_FILENO, buff, n);
    }
}
