#include <stdlib.h>
#include <unistd.h>
#include "apue.h"


int main() {
    pid_t pid;
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {  // child
        exit(0);
    }
    printf("child pid: %d\n", pid);

// `ps' is not very portable utility.
#if defined(__sun__)
    char *cmd_template = "ps -p %d -o s | tail -n 1";
#elif defined(__linux__)
    char *cmd_template = "ps --pid %d -o state --no-header";
#else // assume *BSD
    // Without `-a' ps on FreeBSD and DragonFly doesn't want to select zombie process.
    char *cmd_template = "ps -a -p %d -o stat | tail -n 1";
#endif

    char cmd[256] = { 0 };
    sprintf(cmd, cmd_template, pid);
    printf("Child status (first letter should be `Z'): ");
    fflush(stdout);
    system(cmd);

    return EXIT_SUCCESS;
}
