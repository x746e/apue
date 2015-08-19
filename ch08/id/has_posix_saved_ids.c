#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>


bool is_supported(int sysconf_option);


int main() {
    printf("_POSIX_SAVED_IDS: ");
#ifdef _POSIX_SAVED_IDS
    printf("defined\n");
#else
    printf("not defined\n");
#endif
    printf("Runtime check: %s\n", is_supported(_SC_SAVED_IDS) ? "supported" : "not supported");
    return EXIT_SUCCESS;
}


bool is_supported(int sysconf_option) {
    errno = 0;
    int result;
    if ((result = sysconf(sysconf_option)) == -1) {
        if (errno == 0) {
            // `name` is not supported
            return false;
        }
    }
    return result;
}
