PROGS = pollmsg sendmsg tst tst2

ifeq ($(shell uname),SunOS)
    LDFLAGS += -lxnet
endif

LDFLAGS += -lpthread

include ../../common.mk
