/**
 * Setup signal handler for SIGINT;
 * longjmp from signal handler to main;
 * Check if SIGINT is blocked
 *
 */

#define __BSD_VISIBLE 1
#include "apue.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

jmp_buf env;

void print_sigmask(char* s);

void handler(int num) {
    printf("hello from handler\n");
    print_sigmask("in handler");
    longjmp(env, 1);
}


int main() {

    struct sigaction action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);

    sys_chk(sigaction(SIGINT, &action, NULL));
    
    if (setjmp(env) > 0) {
        printf("jumped to main\n");
    }

    while (1) {
        print_sigmask("blocked in main");
        sleep(3);
    }

    return EXIT_SUCCESS;
}


void print_sigmask(char *s) {
    sigset_t blocked;
    sigprocmask(0, NULL, &blocked);
    printf("%s:", s);
    if (sigismember(&blocked, SIGHUP)) printf(" SIGHUP");
    if (sigismember(&blocked, SIGINT)) printf(" SIGINT");
    if (sigismember(&blocked, SIGQUIT)) printf(" SIGQUIT");
    if (sigismember(&blocked, SIGILL)) printf(" SIGILL");
    if (sigismember(&blocked, SIGTRAP)) printf(" SIGTRAP");
    if (sigismember(&blocked, SIGABRT)) printf(" SIGABRT");
    if (sigismember(&blocked, SIGBUS)) printf(" SIGBUS");
    if (sigismember(&blocked, SIGFPE)) printf(" SIGFPE");
    if (sigismember(&blocked, SIGKILL)) printf(" SIGKILL");
    if (sigismember(&blocked, SIGUSR1)) printf(" SIGUSR1");
    if (sigismember(&blocked, SIGSEGV)) printf(" SIGSEGV");
    if (sigismember(&blocked, SIGUSR2)) printf(" SIGUSR2");
    if (sigismember(&blocked, SIGPIPE)) printf(" SIGPIPE");
    if (sigismember(&blocked, SIGALRM)) printf(" SIGALRM");
    if (sigismember(&blocked, SIGTERM)) printf(" SIGTERM");
#ifdef SIGSTKFLT
    if (sigismember(&blocked, SIGSTKFLT)) printf(" SIGSTKFLT");
#endif
    if (sigismember(&blocked, SIGCHLD)) printf(" SIGCHLD");
    if (sigismember(&blocked, SIGCONT)) printf(" SIGCONT");
    if (sigismember(&blocked, SIGSTOP)) printf(" SIGSTOP");
    if (sigismember(&blocked, SIGTSTP)) printf(" SIGTSTP");
    if (sigismember(&blocked, SIGTTIN)) printf(" SIGTTIN");
    if (sigismember(&blocked, SIGTTOU)) printf(" SIGTTOU");
    if (sigismember(&blocked, SIGURG)) printf(" SIGURG");
    if (sigismember(&blocked, SIGXCPU)) printf(" SIGXCPU");
    if (sigismember(&blocked, SIGXFSZ)) printf(" SIGXFSZ");
    if (sigismember(&blocked, SIGVTALRM)) printf(" SIGVTALRM");
    if (sigismember(&blocked, SIGPROF)) printf(" SIGPROF");
    if (sigismember(&blocked, SIGWINCH)) printf(" SIGWINCH");
    if (sigismember(&blocked, SIGIO)) printf(" SIGIO");
#ifdef SIGPWR
    if (sigismember(&blocked, SIGPWR)) printf(" SIGPWR");
#endif
    if (sigismember(&blocked, SIGSYS)) printf(" SIGSYS");
    printf("\n");
}
