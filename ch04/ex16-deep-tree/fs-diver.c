#include "apue.h"
#include "common.h"
#include <stdio.h>

char target_dir[] = "adir";

int main() {
    int i = 0;
    while (1) {
        printf("Creating directory #%d\n", i++);
        sys_chk(mkdir(target_dir, 0777));
        sys_chk(chdir(target_dir));
    }
    return EXIT_SUCCESS; // Never reached though.
}
