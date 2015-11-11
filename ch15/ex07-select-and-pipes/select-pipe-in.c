#include "apue.h"
#include "common.h"
#include <assert.h>
#include <sys/select.h>

#define BFSZ 128


int main() {
    int pid, pipe_in, pipe_out;
    int pipe_fds[2];
    char msg[] = "hello!";
    char buf[BFSZ];

    sys_chk(pipe(pipe_fds));
    pipe_out = pipe_fds[0];
    pipe_in = pipe_fds[1];

    sys_chk(pid = fork());

    if (pid == 0) {  // Child.
        sys_chk(close(pipe_out));

        assert(write(pipe_in, msg, sizeof msg) == sizeof msg);

        /* fprintf(stderr, "Exiting child\n"); */
        sys_chk(close(pipe_in));
        return EXIT_SUCCESS;

    } else {  // Parent.
        fd_set readfds, writefds, exceptfds;

        sys_chk(close(pipe_in));

        fprintf(stderr, "Sleeping... ");
        sleep(5);
        fprintf(stderr, "Done.\n");

        FD_ZERO(&readfds);
        FD_SET(pipe_out, &readfds);
        FD_ZERO(&writefds);
        FD_SET(pipe_out, &writefds);
        FD_ZERO(&exceptfds);
        FD_SET(pipe_out, &exceptfds);

        sys_chk(select(pipe_out + 1, &readfds, &writefds, &exceptfds, NULL));
        if (FD_ISSET(pipe_out, &readfds))
            printf("Ready for reading\n");
        if (FD_ISSET(pipe_out, &writefds))
            printf("Ready for writing\n");
        if (FD_ISSET(pipe_out, &exceptfds))
            printf("Exception\n");

        sys_chk(read(pipe_out, buf, BFSZ));
        fprintf(stderr, "Got from child: %s\n", buf);
        return EXIT_SUCCESS;
    }
}
