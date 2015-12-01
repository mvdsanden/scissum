DOCUMENT_PATH=$(shell pwd)
export DOCUMENT_PATH

include src/defaults.mk

LDFLAGS=\
$(DOCUMENT_PATH)/src/tools/scissum_tools.a\
$(DOCUMENT_PATH)/src/sci/scissum_sci.a\
$(DOCUMENT_PATH)/src/graphics/scissum_graphics.a\
$(DOCUMENT_PATH)/src/vm/scissum_vm.a

#CXXFLAGS=-std=c++11

all: tools sci graphics vm ResourceScanner ResourceExtractor

tools:
	$(MAKE) -C src/tools

sci:
	$(MAKE) -C src/sci

graphics:
	$(MAKE) -C src/graphics

vm:
	$(MAKE) -C src/vm

ResourceScanner: src/ResourceScanner.cpp sci
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o ResourceScanner src/ResourceScanner.cpp

ResourceExtractor: src/ResourceExtractor.cpp sci
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o ResourceExtractor src/ResourceExtractor.cpp

tests: tools sci graphics vm
	$(MAKE) -C src/tests

clean:
	$(MAKE) -C src/sci clean
	$(MAKE) -C src/tools clean
	$(MAKE) -C src/graphics clean
	$(MAKE) -C src/vm clean
	$(MAKE) -C src/tests clean