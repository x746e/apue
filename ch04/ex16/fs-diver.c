#include "apue.h"
#include "common.h"
#include <stdio.h>

char dirname[] = "adir";

void main() {
    int i = 0;
    while (1) {
        printf("Creating directory #%d\n", i++);
        sys_chk(mkdir(dirname, 0777));
        sys_chk(chdir(dirname));
    }
}
