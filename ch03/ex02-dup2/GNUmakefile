#PROGS = mydup2 check_fd
PROGS = check_fd

UNAME = $(shell uname)

ifeq ($(UNAME),FreeBSD)
    LDFLAGS_check_fd = -lprocstat
    CFILES_check_fd = guess_fd_file_name_freebsd.c
endif
ifeq ($(UNAME),Linux)
    CFILES_check_fd = guess_fd_file_name_linux.c
endif


include ../../common.mk
