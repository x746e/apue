CHAPTERS := $(wildcard ch*)

all: $(CHAPTERS)

$(CHAPTERS):
	$(MAKE) -C $@

test:
	@set -e; for dir in $(CHAPTERS); do \
		$(MAKE) -C $$dir test; \
	done

.PHONY: all $(CHAPTERS)
