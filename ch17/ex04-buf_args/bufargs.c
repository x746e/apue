#include "apue.h"
#include "common.h"

#define MAXARGC     3  /* max number of arguments in buf */
#define WHITE   " \t\n" /* white space for tokenizing arguments */

/*
 * buf[] contains white-space-separated arguments.  We convert it to an
 * argv-style array of pointers, and call the user's function (optfunc)
 * to process the array.  We return -1 if there's a problem parsing buf,
 * else we return whatever optfunc() returns.  Note that user's buf[]
 * array is modified (nulls placed after each token).
 */
int
buf_args(char *buf, int (*optfunc)(int, char **))
{
    char    *ptr;
    int     argc;

    int     argv_len = MAXARGC;
    char    **argv = calloc(argv_len, sizeof(char*));

    if (strtok(buf, WHITE) == NULL)     /* an argv[0] is required */
        return(-1);
    argv[argc = 0] = buf;
    while ((ptr = strtok(NULL, WHITE)) != NULL) {
        if (++argc >= argv_len-1) {    /* -1 for room for NULL at end */
            argv_len *= 2;
            argv = realloc(argv, argv_len * sizeof(char*));
        }
        argv[argc] = ptr;
    }
    argv[++argc] = NULL;

    /*
     * Since argv[] pointers point into the user's buf[],
     * user's function can just copy the pointers, even
     * though argv[] array will disappear on return.
     */
    int ret = ((*optfunc)(argc, argv));
    free(argv);
    return ret;
}


int print(int argc, char *argv[]) {
    for (int i = 0; i < argc; ++i) {
        printf("#%d: %s\n", i, argv[i]);
    }
    return 0;
}


int main() {
    char *buf = malloc(MAXLINE);

    strcpy(buf, "hello world a b c");
    sys_chk(buf_args(buf, print));

    return EXIT_SUCCESS;
}
