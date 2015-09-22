#include <stdlib.h>
#include <sys/time.h>
#include <sys/select.h>


void sleep_us(long useconds) {
    struct timeval timeout = { useconds / 1000000, useconds % 1000000 };
    select(0, NULL, NULL, NULL, &timeout);
}


int main() {
    sleep_us(999999);
}
