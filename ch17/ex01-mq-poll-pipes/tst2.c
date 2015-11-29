#include "apue.h"


int main() {
    struct foo {
        int a;
        char b[128];
    };

    struct foo f;
    struct foo *fp = &f;

    printf("&f:       %p\n", &f);
    printf("fp:       %p\n", fp);
    printf("&(f.a):   %p\n", &(f.a));
    printf("&(f.b):   %p\n", &(f.b));
    printf("fp->b:    %p\n", fp->b);
    printf("&(fb->b): %p\n", &(fp->b));
    return EXIT_SUCCESS;
}
