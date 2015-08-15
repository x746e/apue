/*
 * Write a utility like cp(1) that copies a file containing holes, without
 * writing the bytes of 0 to the output file.
 */
#include "apue.h"
#include "common.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>


bool is_all_zeros(char *buffer, size_t len);


int main (int argc, char *argv[]) {
    if (argc != 3) {
        err_quit("Usage: %s <source> <dest>", argv[0]);
    }

    // Open files.
    int src_fd, dst_fd;
    sys_chk(src_fd = open(argv[1], O_RDONLY));
    sys_chk(dst_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, FILE_MODE));

    // Figure out block size of the target file.
    struct stat st;
    sys_chk(fstat(dst_fd, &st));
    size_t buffer_size = st.st_blksize;
    // Make a buffer of that size.
    char *buffer;
    if ((buffer = malloc(sizeof(char) * buffer_size)) == NULL) {
        err_sys("Cannot allocate a buffer");
    }

    size_t nread;
    do {
        ssize_t m = 0;
        nread = 0;
        // Read a block (or less).
        do {
            sys_chk(m = read(src_fd, buffer + nread, buffer_size - nread));
            nread += m;
        } while (nread < buffer_size && m != 0);
 
        // If it's a full block with zeros..
        if (nread == buffer_size && is_all_zeros(buffer, nread)) {
            // ..lseek past it.
            sys_chk(lseek(dst_fd, nread, SEEK_CUR));
        } else {
            // Write buffer to the target file.
            size_t nwritten = 0;
            while (nwritten < nread) {
                sys_chk(m = write(dst_fd, buffer + nwritten,
                                  nread - nwritten));
                nwritten += m;
            }
        }
    // Repeat until there is something to read from src file.
    } while (nread > 0);

    // Close both files, free the buffer.
    sys_chk(close(src_fd));
    sys_chk(close(dst_fd));
    free(buffer);
}


bool is_all_zeros(char *buffer, size_t len) {
    for (int i = 0; i < len; ++i) {
        if (buffer[i] != '\0') {
            return false;
        }
    }
    return true;
}
