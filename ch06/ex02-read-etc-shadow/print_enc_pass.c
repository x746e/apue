#include "apue.h"
#include <stdio.h>
#include <shadow.h>
#include <errno.h>


int main(int argc, char *argv[]) {
    int i = 0;
    struct spwd *ent;

    setspent();
    errno = 0;
    while (ent = getspent()) {
        ++i;
        printf("%20s%30s\n", ent->sp_namp, ent->sp_pwdp);
        errno = 0;
    }
    if (ent == NULL) {
        err_sys("getspent");
    }
    endspent();

    printf("%d entries overall in /etc/shadow", i);
    return 0;
}
