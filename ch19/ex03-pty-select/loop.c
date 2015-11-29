#include "apue.h"
#include "common.h"
#include <sys/select.h>

#define BUFFSIZE    512
#define MAX(a, b)   ((a) > (b) ? (a) : (b))

void
loop(int ptym, int ignoreeof)
{
    pid_t   child;
    int     nread;
    char    buf[BUFFSIZE];

    fd_set  watch_for_set, changed_set;
    int     nfds;  // highest-numbered file descriptor in the sets, plus 1
    nfds = MAX(ptym, STDIN_FILENO) + 1;

    FD_ZERO(&watch_for_set);
    FD_SET(ptym, &watch_for_set);
    FD_SET(STDIN_FILENO, &watch_for_set);

    for (;;) {
        changed_set = watch_for_set;
        sys_chk(select(nfds, &changed_set, NULL, NULL, NULL));
        if (FD_ISSET(ptym, &changed_set)) {
            if ((nread = read(ptym, buf, BUFFSIZE)) <= 0)
                break; // EOF or child exited so and terminal destroyed?
            if (writen(STDOUT_FILENO, buf, nread) != nread)
                err_sys("writen error to stdout");
        }
        if (FD_ISSET(STDIN_FILENO, &changed_set)) {
            if ((nread = read(STDIN_FILENO, buf, BUFFSIZE)) < 0)
                err_sys("read error from stdin");
            else if (nread == 0)
                break;
            if (writen(ptym, buf, nread) != nread)
                err_sys("writen error to master pty");
        }
    }
}
