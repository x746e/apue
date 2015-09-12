PROGS = sig2str

sig2str: sig2str.c linux_sys_signame.h

linux_sys_signame.h: get_sys_signame.py
	if [ "`uname`" = "Linux" ]; then \
	    python get_sys_signame.py >| linux_sys_signame.h; \
	fi

clean: clean-linux_sys_signame

clean-linux_sys_signame:
	rm -f linux_sys_signame.h

include ../../common.mk
