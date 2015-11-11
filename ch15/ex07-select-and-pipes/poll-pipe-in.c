#include "apue.h"
#include "common.h"
#include <assert.h>
#include <poll.h>

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
        struct pollfd fds[1];

        sys_chk(close(pipe_in));

        fprintf(stderr, "Sleeping... ");
        sleep(5);
        fprintf(stderr, "Done.\n");

        fds[0].fd = pipe_out;
        fds[0].events = POLLIN | POLLOUT;


        sys_chk(poll(fds, 1, 0));
        if (fds[0].revents & POLLIN)
            printf("Ready for reading\n");
        if (fds[0].revents & POLLOUT)
            printf("Ready for writing\n");
        if (fds[0].revents & POLLERR)
            printf("Poll error\n");
        if (fds[0].revents & POLLHUP)
            printf("POLLHUP\n");
        if (fds[0].revents & POLLNVAL)
            printf("POLLNVAL\n");

        sys_chk(read(pipe_out, buf, BFSZ));
        fprintf(stderr, "Got from child: %s\n", buf);
        return EXIT_SUCCESS;
    }
}
