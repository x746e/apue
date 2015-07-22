#include "apue.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>

#ifdef OPEN_MAX
static long openmax = OPEN_MAX;
#else
static long openmax = 0;
#endif

/*
 * If OPEN_MAX is indeterminate, this might be inadequate.
 */
#define OPEN_MAX_GUESS 256

long
open_max(void)
{
    if (openmax == 0) {  /* first time through */
        errno = 0;
        if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
            if (errno == 0) {
                openmax = OPEN_MAX_GUESS;  /* it's indeterminate */
            } else {
                err_sys("sysconf error for _SC_OPEN_MAX");
            }
        } else if (openmax == LONG_MAX) {
            openmax = OPEN_MAX_GUESS;
        }
    }
    return openmax;
}


int
main() {
    printf("open_max: %ld\n", open_max());
    exit(0);
}
