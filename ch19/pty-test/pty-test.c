// Main goal is to see what's happening when the second process is closing
// slave side of pty, and the first process is trying to read from it.

#include "apue.h"
#include "common.h"
#include <fcntl.h>

int main() {

    int ptym;
    sys_chk(ptym = posix_openpt(O_NOCTTY | O_RDWR));
    printf("ptym: %d; ptsname: %s\n", ptym, ptsname(ptym));

    sleep(60);

    return EXIT_SUCCESS;
}
