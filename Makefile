DOCUMENT_PATH=$(shell pwd)
export DOCUMENT_PATH

LDFLAGS=$(DOCUMENT_PATH)/src/tools/scissum_tools.a $(DOCUMENT_PATH)/src/sci/scissum_sci.a
CXXFLAGS=-std=c++11

all: tools sci ResourceScanner ResourceExtractor

tools:
	$(MAKE) -C src/tools

sci:
	$(MAKE) -C src/sci

ResourceScanner: src/ResourceScanner.cpp sci
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o ResourceScanner src/ResourceScanner.cpp

ResourceExtractor: src/ResourceExtractor.cpp sci
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o ResourceExtractor src/ResourceExtractor.cpp

tests: tools
	$(MAKE) -C src/tests

clean:
	$(MAKE) -C src/tools clean
	$(MAKE) -C src/tests clean