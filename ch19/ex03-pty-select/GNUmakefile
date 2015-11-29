# PROGS = main.c

# all:	pty

INCLUDES = -I ../..

CFILES = ../../apue.3e/lib/error.c ../../apue.3e/lib/ptyfork.c ../../apue.3e/lib/ttymodes.c ../../apue.3e/lib/writen.c ../../apue.3e/lib/ptyopen.c ../../apue.3e/lib/signalintr.c ../../apue.3e/lib/spipe.c

%.o: %.c
	$(CC) $(CFLAGS) $(CFILES) $(INCLUDES) -c $< -o $@


pty:	main.o loop.o driver.o
	$(CC) $(CFLAGS) $(CFILES) $(INCLUDES) -o pty main.o loop.o driver.o $(LDFLAGS) $(LDLIBS)
