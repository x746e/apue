#define _GNU_SOURCE
#define __EXTENSIONS__
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <limits.h>
#ifdef __sun__
#include <alloca.h>
#include <sys/signal.h>
#include <sys/procfs.h>
#endif

#include "apue.h"
#include "common.h"

#if !defined(__linux__) && !defined(strdupa)
#define strdupa(__str) \
    strncpy(alloca(strlen(__str) + 1), \
            __str, \
            strlen(__str) + 1);
#endif


#if defined(__FreeBSD__) || defined(__DragonFly__) || defined(__linux__)
#define has_getresuid
#endif


#if defined(__FreeBSD__) || defined(__DragonFly__) || defined(__sun__)
#define has_issetugid
#endif


#ifdef has_getresuid
void get_user_ids(uid_t *real, uid_t *effective, uid_t *saved) {
    sys_chk(getresuid(real, effective, saved));
}
#else
#if defined(__sun__)
void get_user_ids(uid_t *real, uid_t *effective, uid_t *saved) {
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

    *effective = cred.pr_euid;
    *real = cred.pr_ruid;
    *saved = cred.pr_suid;
}
#else
void get_user_ids(uid_t *real, uid_t *effective, uid_t *saved) {
    *real = getuid();
    *effective = geteuid();
    *saved = -1;
}
#endif
#endif


void print_ids() {
    struct passwd *pwent;

    /** Different process ids */
    printf("Process ID: %lld\n", (long long)getpid());
    printf("Parent PID: %lld\n", (long long)getppid());
    printf("Process group ID: %lld\n", (long long)getpgrp());
    printf("Session ID: %lld\n", (long long)getsid(getpid()));
    

    /** Different user ids */
    uid_t real_uid, effective_uid, saved_uid;
    get_user_ids(&real_uid, &effective_uid, &saved_uid);

    // TODO: To distinguish between nonexistend user and error we need to set `errno=0` beforehand.
    sys_ptr_chk(pwent = getpwuid(real_uid));
    char *real_uname = strdupa(pwent->pw_name);

    sys_ptr_chk(pwent = getpwuid(effective_uid));
    char *effective_uname = strdupa(pwent->pw_name);

    // TODO: Handle case when we can't get saved uid.
    sys_ptr_chk(pwent = getpwuid(saved_uid));
    char *saved_uname = strdupa(pwent->pw_name);

    printf("\n");
    printf("Real user ID: %d (%s)\n", real_uid, real_uname);
    printf("Effective user ID: %d (%s)\n", effective_uid, effective_uname);
    printf("Saved user ID: %d (%s)\n", saved_uid, saved_uname);


    /** else */
    printf("\n");
#ifdef has_issetugid
    if (issetugid()) {
        printf("Process is tainted\n");
    } else {
        printf("Process is *not* tainted\n");
    }
#endif
}

int main() {
    print_ids();

    return EXIT_SUCCESS;
}
