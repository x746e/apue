PROGS = pollmsg sendmsg

ifeq ($(shell uname),SunOS)
    LDFLAGS += -lxnet
endif

LDFLAGS += -lpthread

include ../../common.mk
