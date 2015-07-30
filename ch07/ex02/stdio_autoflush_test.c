#include <stdio.h>
#include <unistd.h>
#include <string.h>

void main() {
    int i;
    char tstr[] = "Outputting char by char";
    for (i = 0; i < strlen(tstr); ++i) {
        fputc(tstr[i], stderr);
        sleep(1);
    }

    fputs(">>stdout1<<\n", stdout);
    fputs(">>stdout2<<", stdout);
    fputs(">>stderr1<<", stderr);
}
