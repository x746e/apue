// Turn terminal into raw mode, send to master pty & echo everything typed in.

#include "apue.h"
#include "common.h"
#include <fcntl.h>
#include <stdbool.h>

#define BUFSZ 256


void
sig_restore_tty_mode(int signum) {
    tty_atexit();
}


int main(int argc, char *argv[]) {
    char buf[BUFSZ];
    int n, opt;
    bool raw = false;

    while ((opt = getopt(argc, argv, "r")) != -1) {
        switch (opt) {
            case 'r':
                raw = true;
                break;
            default:
                fprintf(stderr, "Usage: %s [-r]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (raw) {
        sys_chk(atexit(tty_atexit));
        sys_chk(tty_raw(STDIN_FILENO));

        struct sigaction action;
        action.sa_handler = sig_restore_tty_mode;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;
        sys_chk(sigaction(SIGTERM, &action, NULL));
    }

    int ptym;
    sys_chk(ptym = posix_openpt(O_NOCTTY | O_RDWR));
    sys_chk(grantpt(ptym));
    sys_chk(unlockpt(ptym));
    printf("ptym: %d; ptsname: %s\r\n", ptym, ptsname(ptym));

    while(1) {
        sys_chk(n = read(STDIN_FILENO, buf, BUFSZ));

        printf(">> %d:\t", n);
        for (int i=0; i < n; ++i) {
            printf("%02d ", buf[i]);
        }
        printf("\r\n");

        sys_chk(write(ptym, buf, n));
    }

    return EXIT_SUCCESS;
}
