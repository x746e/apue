PROGS = fd-passing

ifeq ($(shell uname),SunOS)
    LDFLAGS += -lxnet
endif

include ../../common.mk

CFLAGS += -DLINUX
CFILES += ../../apue.3e/lib/signal.c ../../apue.3e/lib/sendfd.c ../../apue.3e/lib/recvfd.c ../../apue.3e/lib/tellwait.c
