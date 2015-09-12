PROGS = sig2str

sig2str: sig2str.c linux_sys_signame.h

linux_sys_signame.h: get_sys_signame.py
	python get_sys_signame.py >| linux_sys_signame.h

include ../../common.mk
