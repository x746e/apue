PROGS = atexit-typedef

all: $(PROGS) .gitignore

%: %.c
	cc -std=c99 -g $@.c -o $@ -lm -I ../.. ../../apue.3e/lib/error.c


.gitignore: $(PROGS)
	touch .gitignore
	for prog in $(PROGS); do \
	    if ! grep -s $$prog .gitignore; then \
		echo $$prog >> .gitignore ; \
	    fi \
	done
