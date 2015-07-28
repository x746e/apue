#include <stdio.h>

int const MODE_666 = (S_IRUSR | S_IWUSR |
                      S_IRGRP | S_IWGRP |
                      S_IROTH | S_IWOTH);


#define sys_chk(call) if ((call) == -1) { \
    err_sys("Error in `" #call "` syscall (%s:%d)", __FILE__, __LINE__); \
}

#define sys_ptr_chk(call) if ((call) == NULL) { \
    err_sys("Error in `" #call "` syscall (%s:%d)", __FILE__, __LINE__); \
}

#define sys_eof_chk(call) if ((call) == EOF) { \
    err_sys("Error in `" #call "` syscall (%s:%d)", __FILE__, __LINE__); \
}
