#include <stdlib.h>
#include <errno.h>
#include "apue.h"


long parse_long(char *s) {
    errno = 0;
    long result = strtol(s, NULL, 10);
    if (errno != 0) {
        err_sys("Cannot convert string to number: %s", s);
    }
    return result;
}
