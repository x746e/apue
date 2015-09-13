#include "apue.h"
#include <fcntl.h>

#define BUFFSIZE    100


void sig_xfsz() {
    fprintf(stderr, "Caught SIGXFSZ\n");
}


int
main(void)
{
    int     n, nwritten;
    char    buf[BUFFSIZE];

    int f1 = open("file", O_RDONLY);
    int f2 = open("file.copy", O_WRONLY | O_CREAT | O_TRUNC);

    signal_intr(SIGXFSZ, sig_xfsz);

    while ((n = read(f1, buf, BUFFSIZE)) > 0) {
        if ((nwritten = write(f2, buf, n)) != n) {
            err_sys("write error, write %d bytes", nwritten);
        }
    }

    if (n < 0)
        err_sys("read error");

    exit(0);
}
