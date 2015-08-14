PROGS = test-umask

# Hack to overcome deficiency of `which` on solaris.
PYTHON = $(shell which python2.7 > /dev/null && which python2.7 || which python)


test:
	$(PYTHON) test.py


include ../../common.mk
