#include "apue.h"
#include <syslog.h>

int main() {
    FILE *fp;

    fp = fopen("daemonize-getlogin.log", "w");

    fprintf(fp, "login: %s\n", getlogin());
    daemonize("daemonize-getlogin");
    fprintf(fp, "login: %s\n", getlogin());
    return EXIT_SUCCESS;
}
