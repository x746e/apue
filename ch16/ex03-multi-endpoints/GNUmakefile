PROGS = ruptimed

ifeq ($(shell uname),SunOS)
    LDFLAGS += -lxnet
endif

CFILES_ruptimed = ../../apue.3e/lib/setfd.c ../../apue.3e/lib/daemonize.c initsrv2.c

include ../../common.mk
