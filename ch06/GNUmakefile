EXERCISES := $(wildcard ex*)

all: $(EXERCISES)

$(EXERCISES):
	$(MAKE) -C $@

test:
	for dir in $(EXERCISES); do \
		$(MAKE) -C $$dir test; \
	done


.PHONY: all $(EXERCISES)

