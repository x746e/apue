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

#ifdef __sun__
    char *cmd_template = "ps -p %d -o s | tail -n 1";
#else
    char *cmd_template = "ps -p %d -o stat | tail -n 1";
#endif

    char cmd[256] = { 0 };
    sprintf(cmd, cmd_template, pid);

    printf("Child status (first letter should be `Z'): ");
    fflush(stdout);
    system(cmd);

    return EXIT_SUCCESS;
}
