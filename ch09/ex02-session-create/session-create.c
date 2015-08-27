/*
 * Write a small program that calls fork and has the child create a new
 * session.  Verify that the child no longer has a controlling terminal.
 */
#include "apue.h"
#include "common.h"


void check_ids();


int main() {

    pid_t pid = fork();
    if (pid < 0) {
        err_sys("fork error");
    } else if (pid == 0) {  // child
        check_ids("child before setsid");
        sys_chk(setsid());
        check_ids("child after setsid");
        return EXIT_SUCCESS;
    } else {  // parent
        sleep(1); // Give time for child to run.
        check_ids("parent");
        return EXIT_SUCCESS;
    }

}


void check_ids(char *who) {
    printf("** %s **\n", who);
    printf("pid: %ld\n", (long)getpid());
    printf("ppid: %ld\n", (long)getppid());
    printf("sid: %ld\n", (long)getsid(0));
    printf("pgrp: %ld\t (process group ID)\n", (long)getpgrp());
    printf("tpgrp: %ld\t (process group ID of the foreground process group on controlling terminal of this process)\n", (long)tcgetpgrp(STDIN_FILENO));
    printf("\n");
}
