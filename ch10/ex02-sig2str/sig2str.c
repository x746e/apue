#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>


#ifndef __sun__
#define      SIG2STR_MAX     32
int sig2str(int signum, char *str);
#endif


#ifdef __linux__
  #ifndef NSIG
    #define NSIG _NSIG
  #endif
  #include "linux_sys_signame.h"
#endif


int main(int argc, char *argv[]) {
    /* int signum; */
    /* for (signum = 0; signum < NSIG; ++signum) { */
    /*     printf("%d: %s\n", signum, sys_signame[signum]); */
    /* } */
    int ret;
    char buf[SIG2STR_MAX]; 
    int signo;

    for (signo = -3; signo < 100; ++signo) {
        bzero(buf, SIG2STR_MAX);

        ret = sig2str(signo, buf);
        printf("signo: %d, ret: %d, str: %s\n", signo, ret, buf);
    }

    return EXIT_SUCCESS;
}

#ifndef __sun__
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
#endif
