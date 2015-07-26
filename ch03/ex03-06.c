/*
 * If you open a file for read-write with the append flag, can you still read
 * from anywhere in the file using lseek?  Can use you lseek to replace
 * existing data in the file?  Write a program to verify this.
 */
#include "apue.h"
#include <stdio.h>
#include <fcntl.h>


void check_position(int fd);


void main(int argc, char *argv[]) {
    if (argc != 2) {
        err_quit("Usage: %s <file>", argv[0]);
    }
        
    int fd;
    if ((fd = open(argv[1], O_RDWR /*| O_APPEND*/)) < 0) {
        err_sys("Can't open file `%s`", argv[1]);
    }

    check_position(fd);

    lseek(fd, 100, SEEK_SET);

    char msg[] = "hello there";
    check_position(fd);
    write(fd, msg, strlen(msg));
    check_position(fd);

    char buffer[BUFSIZ] = { 0 };
    if (read(fd, buffer, BUFSIZ) < 0) {
        err_sys("Can't read from fd#%d", fd);
    }

    check_position(fd);
    printf("%s\n", buffer);
}


void check_position(int fd) {
    off_t current_position;
    if ((current_position = lseek(fd, 0, SEEK_CUR)) == -1) {
        err_ret("Can't get current position");
    } else {
        printf("current_position: %zd\n", current_position);
    }
}
