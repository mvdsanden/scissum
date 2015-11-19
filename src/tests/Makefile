include ../defaults.mk

NAME=tests

SOURCES=$(wildcard *.cpp)

OBJECTS=$(patsubst %.cpp,%,$(SOURCES))

LDFLAGS=$(DOCUMENT_PATH)/src/tools/scissum_tools.a

all: $(OBJECTS) $(DOCUMENT_PATH)/src/tools/scissum_tools.a


clean:
	rm $(OBJECTS)
