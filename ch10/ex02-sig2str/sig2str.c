#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
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
    int ret;
    char buf[SIG2STR_MAX]; 
    int signum;

    for (signum = -3; signum < 100; ++signum) {
        bzero(buf, SIG2STR_MAX);

        ret = sig2str(signum, buf);
        printf("signum: %3d, ret: %2d, str: %15s, desc: %s\n", signum, ret, buf, strsignal(signum));
    }

    return EXIT_SUCCESS;
}

#ifndef __sun__
int sig2str(int signum, char *str) {
    if (0 < signum && signum < SIGRTMIN) {
        if (sys_signame[signum] != NULL) {
            strncpy(str, sys_signame[signum], SIG2STR_MAX);
            return 0;
        }
    } else if (SIGRTMIN <= signum && signum <= SIGRTMAX) {
        int rtmiddle = SIGRTMIN + (SIGRTMAX - SIGRTMIN) / 2;
        if (signum == SIGRTMIN) {
            strncpy(str, "RTMIN", SIG2STR_MAX);
        } else if (signum == SIGRTMAX) {
            strncpy(str, "RTMAX", SIG2STR_MAX);
        } else if (signum <= rtmiddle) {
            snprintf(str, SIG2STR_MAX, "RTMIN+%d", signum - SIGRTMIN);
        } else if (signum > rtmiddle) {
            snprintf(str, SIG2STR_MAX, "RTMAX-%d", SIGRTMAX - signum);
        } else {
            assert(0);  // Unreachable
        }
        return 0;
    }

    return -1;
}
#endif
