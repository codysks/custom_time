SHELL = /bin/bash

TEST_TARGETS = test
TEST_DIR = test
.PHONY: all
all:
	$(MAKE) -C src
	$(MAKE) libcustom_time.a
	$(MAKE) -C test

libcustom_time.a:
	ar rcs $@ src/*.o
.PHONY: test
test:
	$(MAKE) all
	./test/test && echo test passed
.PHONY: clean
clean:
	$(MAKE) -C src clean
	$(MAKE) -C test clean
	rm -f *.a
