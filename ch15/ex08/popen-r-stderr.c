#include "apue.h"


int main() {
    popen("echo \"hello\" > /dev/stderr", "r");
    return EXIT_SUCCESS;
}
