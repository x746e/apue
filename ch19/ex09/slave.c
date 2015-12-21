#include "apue.h"
#include "common.h"


void sig_handler(int signum) {
    fprintf(stderr, "Slave: got %s signal\n", strsignal(signum));
    if (signum == SIGWINCH) {
        struct winsize size;
        sys_chk(ioctl(STDIN_FILENO, TIOCGWINSZ, (char *) &size));
        printf("window size: %d rows, %d columns\n", size.ws_row, size.ws_col);
    }
}


int main() {
    printf("hello from child\n");

    struct sigaction act;
    act.sa_handler = sig_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sys_chk(sigaction(SIGTERM, &act, NULL));
    sys_chk(sigaction(SIGWINCH, &act, NULL));

    while(1);
        //pause();

    return EXIT_SUCCESS;
}
