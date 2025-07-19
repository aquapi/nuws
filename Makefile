EXAMPLE_FILES := hello

bindings:
	cd ./capi && $(MAKE) static
	nelua --script ./lib/bindgen.lua

examples: bindings
	cd ./examples && \
	for FILE in $(EXAMPLE_FILES); do \
	  nelua $$FILE.nelua -M -V -T -s -o $$FILE && \
	  nelua $$FILE.nelua -M --print-code > $$FILE.c && \
	  nelua $$FILE.nelua -M --print-assembly > $$FILE.s; \
	done
