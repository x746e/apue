.PHONY: $(SUBDIRS) recurse

test: TARGET=test
clean: TARGET=clean
all: TARGET=all

recurse test clean all: $(SUBDIRS)

$(SUBDIRS):
	    @exec $(MAKE) -C $@ $(TARGET)
