#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <semaphore.h>
#include "apue.h"

#define sys_chk(call) if ((call) == -1) { \
    err_sys("Error in `" #call "` syscall (%s:%d)", __FILE__, __LINE__); \
}

#define sys_ptr_chk(call) if ((call) == NULL) { \
    err_sys("Error in `" #call "` syscall (%s:%d)", __FILE__, __LINE__); \
}

#define sys_eof_chk(call) if ((call) == EOF) { \
    err_sys("Error in `" #call "` syscall (%s:%d)", __FILE__, __LINE__); \
}

#define sys_sem_chk(call) if ((call) == SEM_FAILED) { \
    err_sys("Error in `" #call "` syscall (%s:%d)", __FILE__, __LINE__); \
}


long parse_long(char *s);
#endif
