#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <kvm.h>


char kvm_errbuf[_POSIX2_LINE_MAX] = { 0 };
#define kvm_chk(call) if ((call) == NULL) { \
    fprintf(stderr, "Error in `" #call "` syscall (%s:%d): %s\n", \
            __FILE__, __LINE__, kvm_errbuf); \
    exit(EXIT_FAILURE); \
}


void
guess_file_name(int fd, char *buf, int buflen) {
    kvm_t *kd;
    kvm_chk(kd = kvm_openfiles(NULL, NULL, NULL, O_RDONLY, kvm_errbuf));

    char *files_ptr;
    int cnt;
    kvm_chk(files_ptr = kvm_getfiles(kd, 0, 0, &cnt));
    printf("Count: %d\n", cnt);

    if (kvm_close(kd) == -1) {
        fprintf(stderr, "Error in `kvm_close()`: %s\n", kvm_errbuf);
        exit(EXIT_FAILURE);
    }
}


/* int */
/* main() { */
/*     char buf[PATH_MAX]; */
/*     guess_file_name(0, buf, sizeof buf); */
/*     printf("guessed file name: %s\n", buf); */
/*     return EXIT_SUCCESS; */
/* } */
