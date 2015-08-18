#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#ifdef __sun__
#include <alloca.h>
#endif

#include "apue.h"
#include "common.h"

#ifndef __linux__
#define strdupa(__str) \
    strncpy(alloca(strlen(__str) + 1), \
            __str, \
            strlen(__str) + 1);
#endif


#if defined(__FreeBSD__) || defined(__DragonFly__) || defined(__linux__)
#define has_getresuid
#endif


int print_ids() {
    struct passwd *pwent;
    struct group *grent;

    /** Different process ids */
    printf("Process ID: %d\n", getpid());
    printf("Parent PID: %d\n", getppid());
    printf("Process group ID: %d\n", getpgrp());
    printf("Session ID: %d\n", getsid(getpid()));
    

    /** Different user ids */
    uid_t real_uid = getuid();
    // TODO: To distinguish between nonexistend user and error we need to set `errno=0` beforehand.
    sys_ptr_chk(pwent = getpwuid(real_uid));
    char *real_uname = strdupa(pwent->pw_name);

    uid_t effective_uid = geteuid();
    sys_ptr_chk(pwent = getpwuid(effective_uid));
    char *effective_uname = strdupa(pwent->pw_name);

    printf("\n");
    printf("Real user ID: %d (%s)\n", real_uid, real_uname);
    printf("Effective user ID: %d (%s)\n", effective_uid, effective_uname);

#ifdef has_getresuid
    uid_t _u1, _u2, saved_uid;
    sys_chk(getresuid(&_u1, &_u2, &saved_uid));
    sys_ptr_chk(pwent = getpwuid(saved_uid));
    char *saved_uname = strdupa(pwent->pw_name);
    printf("Saved user ID: %d (%s)\n", saved_uid, saved_uname);
#endif


    /** Different group ids */
    gid_t real_gid = getgid();
    sys_ptr_chk(grent = getgrgid(real_gid));
    char *real_gname = strdupa(grent->gr_name);

    gid_t effective_gid = getegid();
    sys_ptr_chk(grent = getgrgid(effective_gid));
    char *effective_gname = strdupa(grent->gr_name);

    printf("\n");
    printf("Real group ID: %d (%s)\n", real_gid, real_gname);
    printf("Effective group ID: %d (%s)\n", effective_gid, effective_gname);

#ifdef has_getresuid
    gid_t _g1, _g2, saved_gid;
    sys_chk(getresgid(&_g1, &_g2, &saved_gid));
    sys_ptr_chk(grent = getgrgid(saved_gid));
    char *saved_gname = strdupa(grent->gr_name);
    printf("Saved user ID: %d (%s)\n", saved_gid, saved_gname);
#endif


    /** else */
    printf("\n");
    if (issetugid()) {
        printf("Process is tainted\n");
    } else {
        printf("Process is *not* tainted\n");
    }

    return EXIT_SUCCESS;
}
