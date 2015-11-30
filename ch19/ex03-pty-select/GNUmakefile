# PROGS = main.c

# all:	pty

INCLUDES = -I ../..
CFLAGS = -ggdb3 -std=c99
CFILES = ../../apue.3e/lib/error.c ../../apue.3e/lib/ptyfork.c ../../apue.3e/lib/ttymodes.c ../../apue.3e/lib/writen.c ../../apue.3e/lib/ptyopen.c ../../apue.3e/lib/signalintr.c ../../apue.3e/lib/spipe.c

ifeq ($(shell uname),SunOS)
    CFLAGS += -DSOLARIS -D__EXTENSIONS__
    LDFLAGS += -lxnet
endif

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@


pty:	main.o loop.o driver.o
	$(CC) $(CFLAGS) $(CFILES) $(INCLUDES) -o pty main.o loop.o driver.o $(LDFLAGS) $(LDLIBS)

clean:
	rm -f *.o pty
