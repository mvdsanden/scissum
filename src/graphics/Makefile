include ../defaults.mk

NAME=graphics
LIBRARY_NAME=$(PROJECT_NAME)_$(NAME)

CLASSES=\
Palette\
PixelBuffer\
Image\
ImageSaver

OBJECTS=$(addsuffix .o,$(CLASSES))
SOURCES=$(addsuffix .cpp,$(CLASSES))
HEADERS=$(addsuffix .hpp,$(CLASSES))

all: $(OBJECTS)
	$(AR) rvs $(LIBRARY_NAME).a $(OBJECTS)

clean:
	rm -f *.o
	rm -f *.a
	rm -rf *.dSYM
