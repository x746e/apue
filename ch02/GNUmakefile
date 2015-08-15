SUBDIRS := $(shell for d in *; do if [ -f "$$d/GNUmakefile" ]; then echo $$d; fi; done)
include ../recurse.mk
