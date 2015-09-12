#include "apue.h"
#include <signal.h>
#define      SIG2STR_MAX     32


int sig2str(int signum, char *str);

#ifdef __linux__
#include "linux_sys_signame.h"
#endif


int main(int argc, char *argv[]) {
    int signum;
    for (signum = 0; signum < _NSIG; ++signum) {
        printf("%d: %s\n", signum, sys_signame[signum]);
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

    /* strcpy(str, strsignal(signum)); */
    return 0;
}
