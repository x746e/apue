#define __EXTENSIONS__

#include <sys/signal.h>
#include <sys/procfs.h>

#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "apue.h"
#include "common.h"


int main() {
    char cred_file_path[PATH_MAX];
    struct prcred cred;

    // TODO: error-checking snprintf macros
    int n;
    n = snprintf(cred_file_path, PATH_MAX, "/proc/%lu/cred", (long)getpid());
    if (n >= PATH_MAX) {
        err_quit("snprintf overflowed the buffer");
    } else if (n < 0) {
        err_sys("snprintf");
    }

    int fd;
    sys_chk(fd = open(cred_file_path, O_RDONLY));

    sys_chk(read(fd, &cred, sizeof cred));

    printf("euid: %lu\n", (long)cred.pr_euid);
    printf("ruid: %lu\n", (long)cred.pr_ruid);
    printf("suid: %lu\n", (long)cred.pr_suid);

    return EXIT_SUCCESS;
}
