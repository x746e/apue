#include "apue.h"
#include <errno.h>

static void sig_pipe(int);      /* our signal handler */

int
main(int argc, char *argv[])
{
    int     fd1[2], fd2[2];
    pid_t   pid;
    char    line[MAXLINE];
    char    *line_ptr = line;
    FILE    *child_in, *child_out;

    if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
        err_sys("signal error");

    if (pipe(fd1) < 0 || pipe(fd2) < 0)
        err_sys("pipe error");

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {                           /* parent */
        close(fd1[0]);
        close(fd2[1]);

        child_in = fdopen(fd1[1], "w");
        child_out = fdopen(fd2[0], "r");
        setvbuf(child_in, NULL, _IOLBF, 0);
        setvbuf(child_out, NULL, _IOLBF, 0);

        while (fgets(line, MAXLINE, stdin) != NULL) {
            /* fprintf(stderr, "%s: read:\n>>>%s<<<\n", argv[0], line); */

            if (fputs(line, child_in) == EOF)
                err_sys("write error to pipe");

            errno = 0;
            if ((line_ptr = fgets(line_ptr, MAXLINE, child_out)) == NULL) {
                if (errno != 0) {
                    err_sys("read error from pipe");
                }
                err_msg("child closed pipe");
                break;
            }

            if (fputs(line_ptr, stdout) == EOF)
                err_sys("fputs error");
        }

        if (ferror(stdin))
            err_sys("fgets error on stdin");
        exit(0);
    } else {                                    /* child */
        close(fd1[1]);
        close(fd2[0]);
        if (fd1[0] != STDIN_FILENO) {
            if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO)
                err_sys("dup2 error to stdin");
            close(fd1[0]);
        }

        if (fd2[1] != STDOUT_FILENO) {
            if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO)
                err_sys("dup2 error to stdout");
            close(fd2[1]);
        }
        if (execl("./add2", "add2", (char *)0) < 0)
            err_sys("execl error");
    }
    exit(0);
}

static void
sig_pipe(int signo)
{
    printf("SIGPIPE caught\n");
    exit(1);
}
