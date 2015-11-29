.PHONY: test clean

all: $(PROGS) .gitignore

ifndef CFLAGS
CFLAGS = -std=c99 -ggdb3 -Wall
endif
INCLUDES = -I ../..
# Hack to overcome deficiency of `which` on solaris.
PYTHON = $(shell which python2.7 > /dev/null && which python2.7 || which python)

ifeq ($(shell uname),FreeBSD)
	CFLAGS += -DBSD -D__BSD_VISIBLE
endif

ifndef CFILES
    CFILES += ../../apue.3e/lib/error.c ../../common.c
endif

%: %.c
	cc $(CFLAGS) $(INCLUDES) $(CFILES) $(CFILES_$@) $< -o $@ $(LDFLAGS) $(LDFLAGS_$@)

test:
	if [ -f test.py ]; then \
		PYTHONPATH=../.. $(PYTHON) test.py ; \
	fi

.gitignore: $(PROGS)
	touch .gitignore
	for prog in $(PROGS); do \
		if ! grep -sq ^$$prog\$$ .gitignore$ ; then \
			echo $$prog >> .gitignore ; \
		fi \
	done

clean:
	rm -f $(PROGS) $(CLEAN_FILES)

.SILENT: .gitignore
