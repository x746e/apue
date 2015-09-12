.PHONY: test clean

sig2str: sig2str.c linux_sys_signame.h
	cc -g -Wall $< -o $@

linux_sys_signame.h: get_sys_signame.py
	if [ "`uname`" = "Linux" ]; then \
	    python get_sys_signame.py >| linux_sys_signame.h; \
	fi

clean: 
	rm -f sig2str linux_sys_signame.h
