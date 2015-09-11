#include <signal.h>
#include "apue.h"
#define      SIG2STR_MAX     32


int sig2str(int signum, char *str);



int main(int argc, char *argv[]) {
    int signum;
    printf("null? %s\n", NULL);
    for (signum = -3; signum < 100; ++signum) {
        printf("%d: %s: %s\n", signum, sys_signame[signum], strsignal(signum));
    }
    return EXIT_SUCCESS;
}


int sig2str(int signum, char *str) {
    // if 0 <= signum < RTMIN:
    //      get signal name
    //      copy it to str
    //      return 0
    // elif RTMIN <= signum < RTMIN + (RTMAX - RTMIN) / 2
    //      copy RTMIN+X to str
    //      return 0
    // elif RTMIN + (RTMAX - RMIN) / 2 <= signo <= RTMAX
    //      copy RTMAX-X to str
    //      return 0
    // else 
    //      return -1

    strcpy(str, strsignal(signum));
    return 0;
}
