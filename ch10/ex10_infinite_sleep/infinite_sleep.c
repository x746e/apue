#include "apue.h"
#include <stdbool.h>
#include <time.h>


int main() {
    time_t t;
    struct tm *tm;

    for (int i = 0; ; ++i) {
        if (i % 5 == 0) {
            t = time(NULL);
            tm = localtime(&t);
            printf("%d\n", tm->tm_sec);
        }
        sleep(60);
    }
    return EXIT_SUCCESS;
}
