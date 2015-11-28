PROGS = client server

ifeq ($(shell uname),SunOS)
    LDFLAGS += -lxnet -lnsl
endif

include ../../common.mk
