include ../defaults.mk

NAME=tests

SOURCES=$(wildcard *.cpp)

OBJECTS=$(patsubst %.cpp,%,$(SOURCES))

LDFLAGS=\
$(DOCUMENT_PATH)/src/tools/scissum_tools.a\
$(DOCUMENT_PATH)/src/utils/scissum_utils.a\
$(DOCUMENT_PATH)/src/vm/scissum_vm.a

all: $(OBJECTS) $(DOCUMENT_PATH)/src/tools/scissum_tools.a $(DOCUMENT_PATH)/src/utils/scissum_utils.a $(DOCUMENT_PATH)/src/vm/scissum_vm.a


clean:
	rm -f $(OBJECTS)
	rm -f *.o
	rm -f *.a
	rm -rf *.dSYM
