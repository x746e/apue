#define _GNU_SOURCE
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include "apue.h"
#include "common.h"


void
print_status(siginfo_t infop) {
    switch (infop.si_code) {
        case CLD_EXITED:  // child called _exit(2)
            printf("normal termination, exit status = %d\n", infop.si_status);
            break;
        case CLD_KILLED:  // child killed by signal
            printf("abnormal termination, signal number = %d\n", infop.si_status);
            break;
        case CLD_DUMPED:  // child killed by signal,  and dumped core
            printf("abnormal termination, signal number = %d (core file generated)\n",
                   infop.si_status);
            break;
        default:
            err_quit("Unexpected infop.si_code %d\n", infop.si_code);
    }
}

int
main(void)
{
    setlinebuf(stdout);

    int num = 1;
    pid_t   pid;
    siginfo_t infop;


    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)              /* child */
        exit(7);

    sys_chk(waitid(P_PID, pid, &infop, WEXITED | WSTOPPED));  /* wait for child */
    print_status(infop);                /* and print its status */


    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)              /* child */
        abort();                    /* generates SIGABRT */

    sys_chk(waitid(P_PID, pid, &infop, WEXITED | WSTOPPED));  /* wait for child */
    print_status(infop);                /* and print its status */


    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)              /* child */
        num /= 0;                /* divide by 0 generates SIGFPE */

    sys_chk(waitid(P_PID, pid, &infop, WEXITED | WSTOPPED));  /* wait for child */
    print_status(infop);                /* and print its status */


    exit(0);
}
