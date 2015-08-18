#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

#include "apue.h"
#include "common.h"


int main() {
    struct passwd *pwent;
    struct group *grent;

    printf("Process ID: %d\n", getpid());
    printf("Parent PID: %d\n", getppid());
    printf("Process group ID: %d\n", getpgrp());
    printf("Session ID: %d\n", getsid(getpid()));
    

    uid_t uid = getuid();
    sys_ptr_chk(pwent = getpwuid(uid));
    char *real_uname = strdupa(pwent->pw_name);

    uid_t euid = geteuid();
    sys_ptr_chk(pwent = getpwuid(euid));
    char *effective_uname = strdupa(pwent->pw_name);

    printf("\n");
    printf("Real user ID: %d (%s)\n", uid, real_uname);
    printf("Effective user ID: %d (%s)\n", euid, effective_uname);


    gid_t gid = getgid();
    sys_ptr_chk(grent = getgrgid(gid));
    char *real_gname = strdupa(grent->gr_name);

    gid_t egid = getegid();
    sys_ptr_chk(grent = getgrgid(egid));
    char *effective_gname = strdupa(grent->gr_name);

    printf("\n");
    printf("Real group ID: %d (%s)\n", gid, real_gname);
    printf("Effective group ID: %d (%s)\n", egid, effective_gname);

    return EXIT_SUCCESS;
}
