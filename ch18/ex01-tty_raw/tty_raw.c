#include "apue.h"

int main() {
    tty_raw(STDIN_FILENO);
    return EXIT_SUCCESS;
}
