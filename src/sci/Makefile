include ../defaults.mk

NAME=sci
LIBRARY_NAME=$(PROJECT_NAME)_$(NAME)

CLASSES=\
Sci32ResourceFileScanner\
Sci32Picture\
Sci32Cel\
Sci32PictureCel\
Sci32ViewCel\
Sci32View\
Sci32Script\
Sci32PaletteLoader\
Sci32PictureLoader\
Sci32ViewLoader\
Sci32ScriptLoader\
Sci32ScriptWriter\
ResourceManager

OBJECTS=$(addsuffix .o,$(CLASSES))
SOURCES=$(addsuffix .cpp,$(CLASSES))
HEADERS=$(addsuffix .hpp,$(CLASSES))

all: $(OBJECTS)
	$(AR) rvs $(LIBRARY_NAME).a $(OBJECTS)

clean:
	rm -f *.o
	rm -f *.a
	rm -rf *.dSYM
