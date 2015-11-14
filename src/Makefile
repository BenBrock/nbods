.SUFFIXES: .c .h .hpp .cpp .o

# Set up the environment
CC=g++
CFLAGS=
LDFLAGS=-lcairo -lm -lX11
INCLUDE=-I/usr/include/cairo

# Define the deps
SOURCES=graphics/displayimage.c main.c
OBJECTS=$(SOURCES:.c=.o)

# Ready
EXECUTABLE=nbody

.PHONY: all clean

all: $(OBJECTS) $(EXECUTABLE)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(INCLUDE) $(OBJECTS) -o $@
	rm -rf $(OBJECTS)

.c.o:
	$(CC) $< -c -o $@ $(INCLUDE) $(CFLAGS)
