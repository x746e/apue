#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <sys/resource.h>
#include <math.h>
#include <fcntl.h>

void main() {
    if (symlink("/", "/dev/fd/42") < 0) {
        perror("symlink");
    }
}
