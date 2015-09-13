#include "apue.h"

#define BUFFSIZE    100


void sig_xfsz() {
    printf("Caught SIGXFSZ\n");
}


int
main(void)
{
    int     n, nwritten;
    char    buf[BUFFSIZE];

    signal_intr(SIGXFSZ, sig_xfsz);

    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
        if ((nwritten = write(STDOUT_FILENO, buf, n)) != n) {
            err_sys("write error, write %d bytes", nwritten);
        }
    }

    if (n < 0)
        err_sys("read error");

    exit(0);
}
