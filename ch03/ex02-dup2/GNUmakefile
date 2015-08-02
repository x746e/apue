#PROGS = mydup2 check_fd
PROGS = check_fd

UNAME = $(shell uname)

ifeq ($(UNAME),FreeBSD)
    CFILES_check_fd = guess_fd_file_name_freebsd.c
else
endif
LDFLAGS_check_fd = -lprocstat -lm


include ../../common.mk
