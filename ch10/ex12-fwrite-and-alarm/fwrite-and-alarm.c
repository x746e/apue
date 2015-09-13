#include "apue.h"


void sig_alrm() {
    printf("SIGALRM caught\n");
}


int main() {
    signal(SIGALRM, sig_alrm);

    char *buff;
    size_t buff_size = (size_t)1024 * (size_t)1024 * (size_t)1024 * (size_t)2;
    buff = malloc(buff_size);

    FILE *f = fopen("file", "w");

    alarm(1);
    fwrite(buff, 1, buff_size, f);

    return EXIT_SUCCESS;
}
