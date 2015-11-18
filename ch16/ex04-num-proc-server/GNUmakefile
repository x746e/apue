PROGS = nprocd

ifeq ($(shell uname),SunOS)
    LDFLAGS += -lxnet
endif

CFILES_nprocd = ../../apue.3e/lib/setfd.c ../../apue.3e/lib/daemonize.c ../../apue.3e/sockets/initsrv2.c

include ../../common.mk
