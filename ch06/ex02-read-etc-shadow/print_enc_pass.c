#include "apue.h"
#include <stdio.h>
#include <errno.h>

#if defined(__sun__) || defined(__linux__)
#include <shadow.h>
int main(int argc, char *argv[]) {
    int i = 0;
    struct spwd *ent;

    setspent();
    errno = 0;
    while ((ent = getspent())) {
        ++i;
        printf("%20s\t%30s\n", ent->sp_namp, ent->sp_pwdp);
        errno = 0;
    }
    if (ent == NULL && errno != 0) {
        err_sys("getspent");
    }
    endspent();

    printf("%d password entries read\n", i);
    return EXIT_SUCCESS;
}
#else
#include <pwd.h>
int main(int argc, char *argv[]) {
    int i = 0;
    struct passwd *ent;

    if (getuid() != 0) {
        err_quit("Should be run as root to read passwords");
    }

    setpwent();
    errno = 0;
    while ((ent = getpwent())) {
        ++i;
        printf("%20s\t%30s\n", ent->pw_name, ent->pw_passwd);
        errno = 0;
    }
    if (ent == NULL && errno != 0) {
        err_sys("getpwent");
    }
    endpwent();

    printf("%d password entries read\n", i);
    return EXIT_SUCCESS;
}
#endif
