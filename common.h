int const MODE_666 = (S_IRUSR | S_IWUSR |
                      S_IRGRP | S_IWGRP |
                      S_IROTH | S_IWOTH);


#define sys_chk(call) if ((call) == -1) { \
    err_sys("Error in `" #call "` syscall (%s:%d)", __FILE__, __LINE__); \
}
