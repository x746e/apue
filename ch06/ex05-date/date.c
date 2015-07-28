#include "apue.h"
#include "common.h"
#include <time.h>


#define MAX_DATE_STRING_LENGTH 100

#define chk_strftime(s, s_size, format, tm) \
    if (strftime(s, s_size, format, tm) == 0) { \
        err_quit("String produced by `strftime` is longer than %d, " \
                 "and doesn't fit into the buffer.  Or `strftime` returned " \
                 "zero sized string, is it possible with format %s?", \
                 s_size, format); \
    }


int main(int argc, char *argv[]) {
    time_t timestamp;
    struct tm *timestruct;
    char formatted_date[MAX_DATE_STRING_LENGTH];

    sys_chk(timestamp = time(NULL));
    sys_ptr_chk(timestruct = localtime(&timestamp));
    chk_strftime(formatted_date, MAX_DATE_STRING_LENGTH,
                 "%a %b %d %H:%M:%S %Z %Y\n", timestruct);

    sys_eof_chk(fputs(formatted_date, stdout));

    return EXIT_SUCCESS;
}
