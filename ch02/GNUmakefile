SUBDIRS := $(shell for d in ex*; do if [ -f "$$d/GNUmakefile" ]; then echo $$d; fi; done)
include ../recurse.mk
