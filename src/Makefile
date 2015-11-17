.SUFFIXES: .cpp .hpp .o

# Set up the environment
CC=g++
CFLAGS=-O3
LDFLAGS=-lcairo -lm -lX11
INCLUDE=-I/usr/include/cairo

SOURCES=timing/timing.cpp graphics/displayimage.cpp tree.cpp phys.cpp nbody.cpp
OBJECTS=$(SOURCES:.cpp=.o)

# Ready
EXECUTABLE=nbody

.PHONY: all clean

all: $(OBJECTS) $(EXECUTABLE)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) $(OBJECTS) -o $@
	rm -rf $(OBJECTS)

.cpp.o:
	$(CC) $< -c -o $@ $(INCLUDE) $(CFLAGS)
