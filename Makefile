DOCUMENT_PATH=$(shell pwd)
export DOCUMENT_PATH

all: tools

tools:
	$(MAKE) -C src/tools

tests: tools
	$(MAKE) -C src/tests

clean:
	$(MAKE) -C src/tools clean
	$(MAKE) -C src/tests clean