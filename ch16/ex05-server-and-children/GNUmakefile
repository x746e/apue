PROGS = ruptimed-fd

ifeq ($(shell uname),SunOS)
    LDFLAGS += -lxnet
endif

CFILES_ruptimed-fd = ../../apue.3e/lib/setfd.c ../../apue.3e/lib/daemonize.c ../../apue.3e/sockets/initsrv2.c

include ../../common.mk
