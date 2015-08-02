all: $(PROGS) .gitignore

%: %.c
	cc -std=c99 -g $@.c -o $@ -lm -I ../.. ../../apue.3e/lib/error.c $(cfiles)

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
