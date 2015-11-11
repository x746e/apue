#include "apue.h"

int
main(int argc, char *argv[])
{
    int     ret, int1, int2;
    char    line[MAXLINE];

    /* fprintf(stderr, "%s: at the start of main\n", argv[0]); */

    setvbuf(stdin, NULL, _IOLBF, 0);
    setvbuf(stdout, NULL, _IOLBF, 0);

    while (1) {
        ret = scanf("%d%d", &int1, &int2);
        /* fprintf(stderr, "%s: got %d from scanf\n", argv[0], ret); */
        if (ret == 2) {
            sprintf(line, "%d\n", int1 + int2);
            if (printf(line) != strlen(line))
                err_sys("write error");
        } else if (ret == EOF) {
            break;
        } else {
            if (printf("invalid args\n") != 13) {
                err_sys("write error");
            }
        }
    }

    exit(EXIT_SUCCESS);
}
