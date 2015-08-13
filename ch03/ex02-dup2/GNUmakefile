
PROGS = mydup2 check_fd

UNAME = $(shell uname)

ifeq ($(UNAME),FreeBSD)
    LDFLAGS += -lprocstat
    GUESS_FD_FILE_NAME = guess_fd_file_name_freebsd.c
else
    ifeq ($(UNAME),Linux)
	GUESS_FD_FILE_NAME = guess_fd_file_name_linux.c
    else
	GUESS_FD_FILE_NAME = guess_fd_file_name_dummy.c
    endif
endif

CFILES_mydup2 = check_fd.c $(GUESS_FD_FILE_NAME)
CFILES_check_fd = check_fd_main.c $(GUESS_FD_FILE_NAME)

include ../../common.mk
