#include "apue.h"
#include "common.h"
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif


char *lock_file_path = "file.lock";
int fd;


void TELL_WAIT() {
    sys_chk(fd = open(lock_file_path, O_WRONLY | O_CREAT | O_TRUNC, FILE_MODE));
    sys_chk(write(fd, "pc", 2));

    sys_chk(write_lock(fd, 0, SEEK_SET, 1));
}


void TELL_PARENT(pid_t pid) {
    sys_chk(write_lock(fd, 1, SEEK_SET, 1));
}


void WAIT_PARENT() {
    writew_lock(fd, 0, SEEK_SET, 1);
}


void TELL_CHILD(pid_t pid) {
    sys_chk(un_lock(fd, 0, SEEK_SET, 1));
}


void WAIT_CHILD() {
    while (true) {
        // Waiting while child will lock the file.
        //printf("In WAIT_CHILD(), checking lock\n");
        if (!is_write_lockable(fd, 0, SEEK_SET, 0)) {
            break;
        } else {
            sleep(1);
        }

    }
}


void test_parent_waiting() {
    pid_t pid;
    TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork() error");
    } else if (pid == 0) {  // child
        printf("In child\n");
        sleep(1);
        TELL_PARENT(getppid());
        printf("Exiting from child\n");
        sleep(5);
        return;
    } else {  // parent
        printf("In parent\n");
        WAIT_CHILD();
        printf("Exiting from parent\n");
        return;
    }
}


void test_child_waiting() {
    pid_t pid;
    TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork() error");
    } else if (pid == 0) {  // child
        printf("In child\n");
        sleep(1);
        WAIT_PARENT();
        printf("Exiting from child\n");
        return;
    } else {  // parent
        printf("In parent\n");
        sleep(1);
        TELL_CHILD(pid);
        printf("Exiting from parent\n");
        return;
    }
}


int main() {
    test_child_waiting();
}
