CC = g++
SRCDIR = src
BUILDDIR = build
TARGET = bin/main

SRCEXT = cpp
SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS = -g -Wall -std=c++11

LIB =
INC =

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@mkdir -p bin
	$(CC) $^ -o $(TARGET) $(LIB)
      
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
 
clean:
	@echo " Cleaning.."
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean