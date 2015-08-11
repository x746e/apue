include ../../common.mk

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


check_fd: check_fd.c check_fd_main.c $(GUESS_FD_FILE_NAME)
	cc $(CFLAGS) $(INCLUDES) $(CFILES) $? -o $@ $(LDFLAGS)

mydup2: mydup2.c check_fd.c $(GUESS_FD_FILE_NAME)
	cc $(CFLAGS) $(INCLUDES) $(CFILES) $? -o $@ $(LDFLAGS)


.gitignore:
	printf 'mydup2\ncheck_fd\n' >| .gitignore

clean:
	rm -f check_fd mydup2


all: check_fd mydup2 .gitignore
