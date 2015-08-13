/*
 * If you open a file for read-write with the append flag, can you still read
 * from anywhere in the file using lseek?  Can use you lseek to replace
 * existing data in the file?  Write a program to verify this.
 */
#include "apue.h"
#include "common.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


void check_position(int fd);
void check_contents(int fd);

// Try reading from different places.
// Try writing in the middle

char testfile[] = "testfile";


int main(int argc, char *argv[]) {
    int fd;

    // Create file with a..z letters.
    sys_chk((fd = open(testfile, O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)));
    for (char c = 'a'; c <= 'z'; c++) {
        sys_chk(write(fd, &c, 1));
    }

    check_contents(fd);
    sys_chk(close(fd));

    // Open it read-write with the append flag.
    sys_chk(fd = open(testfile, O_RDWR | O_APPEND));
    check_position(fd);

    printf("Setting position in the middle of the file\n");
    sys_chk(lseek(fd, 10, SEEK_SET));
    check_position(fd);

    char msg[] = "hello there";
    printf("Writing %lld symbols\n", (long long)strlen(msg));
    write(fd, msg, strlen(msg));
    check_position(fd);

    check_contents(fd);
    check_position(fd);

    sys_chk(close(fd))
}


void check_position(int fd) {
    off_t current_position;
    if ((current_position = lseek(fd, 0, SEEK_CUR)) == -1) {
        err_ret("Can't get current position");
    } else {
        printf("current_position: %lld\n", (long long)current_position);
    }
}

void check_contents(int fd) {
    char buf[BUFSIZ] = { 0 };
    off_t saved_position;
    sys_chk(saved_position = lseek(fd, 0, SEEK_CUR));
    sys_chk(lseek(fd, 0, SEEK_SET));

    read(fd, buf, BUFSIZ);
    printf("File contents (%lld bytes): %s\n", (long long)strlen(buf), buf);

    sys_chk(lseek(fd, saved_position, SEEK_SET));
}
