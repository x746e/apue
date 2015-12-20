#include "apue.h"
#include "common.h"


#define MAX_NAME_LEN 128
#define BUFSZ 128


int ptym_fd;


void slrd() {
    int n;
    char buf[BUFSZ] = { 0 };
    sleep(1);
    sys_chk(n = read(ptym_fd, buf, BUFSZ));
    buf[n] = '\0';
    printf("got from child: %s\n", buf);
}


int main() {
    pid_t pid;
    char slave_name[MAX_NAME_LEN];
    struct winsize size = {
        3, 4
    };

    sys_chk(pid = pty_fork(&ptym_fd, slave_name, MAX_NAME_LEN, NULL, NULL));

    if (pid == 0) {  // Child.
        execl("slave", "slave", (char*) 0);
        return EXIT_SUCCESS;

    } else {  // Parent.
        printf("hello from parent\n");

        slrd();

        // send sigterm
        // Doesn't work on Linux, failing with EINVAL
        if (ioctl(ptym_fd, TIOCSIG, SIGTERM) == 0) {
            slrd();
        }
        
        // change window size
        sys_chk(ioctl(ptym_fd, TIOCSWINSZ, &size));
        slrd();

        return EXIT_SUCCESS;
    }
}
