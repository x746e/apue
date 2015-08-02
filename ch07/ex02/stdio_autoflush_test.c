#include <stdio.h>
#include <unistd.h>
#include <string.h>


int main() {
    int i;
    char sout[] = "out";
    char serr[] = "err";
    for (i = 0; i < strlen(sout); ++i) {
        fputc(sout[i], stdout);
        sleep(1);
    }
    for (i = 0; i < strlen(serr); ++i) {
        fputc(serr[i], stderr);
        sleep(1);
    }

    fputs(">>stdout1<<\n", stdout);
    fputs(">>stdout2<<", stdout);
    fputs(">>stderr1<<", stderr);
}
