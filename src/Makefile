.SUFFIXES: .c .h .o

# Set up the environment
CC=clang
CFLAGS=-c
LDFLAGS=-lcairo -lm -lX11
INCLUDE=-I/usr/include/cairo

# Define the deps
SOURCES=graphics/displayimage.c main.c tree.c
OBJECTS=$(SOURCES:.c=.o)

# Ready
EXECUTABLE=nbody

.PHONY: all clean

all: $(OBJECTS) $(EXECUTABLE)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(CUDAOBJECTS)
	$(CC) $(LDFLAGS) $(INCLUDE) $(OBJECTS) -o $@
	rm -rf $(OBJECTS)

.c.o:
	$(CC) $< -o $@ $(INCLUDE) $(CFLAGS)
