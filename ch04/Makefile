PROGS = readdir ex04-01-filetype ex04-04-umask

all: $(PROGS)

%: %.c
	cc -std=c99 -g $@.c -o $@ -lm -I .. ../apue.3e/lib/error.c
