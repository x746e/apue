#include "apue.h"
#include "common.h"
#include <pthread.h>
#include <stdlib.h>


pthread_mutex_t putenv_lock = PTHREAD_MUTEX_INITIALIZER;


int putenv_r(char *string) {
    int res, err;
    sigset_t new_sigset;
    sigset_t old_sigset;
    sys_chk(sigfillset(&new_sigset));
    sys_chk(sigprocmask(SIG_BLOCK, &new_sigset, &old_sigset));

    err = pthread_mutex_lock(&putenv_lock);
    if (err != 0)
        err_exit(err, "Mutex lock failed");
    res = putenv(string);
    err = pthread_mutex_unlock(&putenv_lock);
    if (err != 0)
        err_exit(err, "Mutex unlock failed");

    sys_chk(sigprocmask(SIG_SETMASK, &old_sigset, NULL));

    return res;
}


int main() {
    putenv_r("HELLO=world");
    system("env|grep HELLO");
    return EXIT_SUCCESS;
}
