all: $(PROGS) .gitignore

CFLAGS = -std=c99 -g -Wall
INCLUDES = -I ../..

CFILES += ../../apue.3e/lib/error.c 

%: %.c
	cc $(CFLAGS) $(INCLUDES) $(CFILES) $(CFILES_$@) $< -o $@ $(LDFLAGS) $(LDFLAGS_$@)

.gitignore: $(PROGS)
	touch .gitignore
	for prog in $(PROGS); do \
	    if ! grep -sq ^$$prog .gitignore$ ; then \
		echo $$prog >> .gitignore ; \
	    fi \
	done

clean:
	rm -f $(PROGS)

.SILENT: .gitignore
