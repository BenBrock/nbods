.SUFFIXES: .cpp .hpp .o

# Set up the environment
CC=clang++
CFLAGS=
LDFLAGS=-lcairo -lm -lX11
INCLUDE=-I/usr/include/cairo

SOURCES=graphics/displayimage.cpp nbody.cpp tree.cpp
OBJECTS=$(SOURCES:.cpp=.o)

# Ready
EXECUTABLE=nbody

.PHONY: all clean

all: $(OBJECTS) $(EXECUTABLE)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(INCLUDE) $(OBJECTS) -o $@
	rm -rf $(OBJECTS)

.cpp.o:
	$(CC) $< -c -o $@ $(INCLUDE) $(CFLAGS)
