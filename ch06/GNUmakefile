EXERCISES := $(wildcard ex*)

all: $(EXERCISES)

$(EXERCISES):
	$(MAKE) -C $@

test:
	@set -e; for dir in $(EXERCISES); do \
		if [ -f $$dir/GNUmakefile ]; then \
			$(MAKE) -C $$dir test; \
		fi \
	done


.PHONY: all $(EXERCISES)

