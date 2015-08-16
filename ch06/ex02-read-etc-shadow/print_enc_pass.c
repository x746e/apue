#include "apue.h"
#include <stdio.h>
#include <errno.h>

#if defined(__sun__) || defined(__linux__)
#define has_spwd 1
#endif

#if has_spwd
#include <shadow.h>
#else
#include <pwd.h>
#endif


#if has_spwd
    #define PW_GET_NAME(ent) ent->sp_namp
    #define PW_GET_PASSWD(ent) ent->sp_pwdp
    #define PW_ENT_T struct spwd

    #define SETENT setspent
    #define GETENT getspent
    #define ENDENT endspent
#else
    #define PW_GET_NAME(ent) ent->pw_name
    #define PW_GET_PASSWD(ent) ent->pw_passwd
    #define PW_ENT_T struct passwd

    #define SETENT setpwent
    #define GETENT getpwent
    #define ENDENT endpwent
#endif

#define FOREACH_PW(pw_var) \
    SETENT(); \
    errno = 0; \
    while ((pw_var = GETENT())) { \
        errno = 0;

#define END_FOREACH_PW(pw_var) \
    } \
    if (pw_var == NULL && errno != 0) { \
        err_sys("getpwent/getspent"); \
    } \
    ENDENT();


int main(int argc, char *argv[]) {
    int i = 0;
    PW_ENT_T *ent;

    if (getuid() != 0) {
        err_quit("Should be run as root to read passwords");
    }

    FOREACH_PW(ent)
        ++i;
        printf("%20s\t%30s\n", PW_GET_NAME(ent), PW_GET_PASSWD(ent));
    END_FOREACH_PW(ent)

    printf("%d password entries read\n", i);
    return EXIT_SUCCESS;
}
