PROGS = apue-ftw ftw

all: $(PROGS)

%: %.c
	cc -std=c99 -g $@.c -o $@ -lm -I ../.. ../../apue.3e/lib/error.c ../../apue.3e/lib/pathalloc.c
