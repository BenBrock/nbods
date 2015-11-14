<<<<<<< HEAD
.SUFFIXES: .c .h .hpp .cpp .o

# Set up the environment
CC=g++
CFLAGS=
=======
.SUFFIXES: .c .h .o

# Set up the environment
CC=clang++
CFLAGS=-c
>>>>>>> Changedd it to da refactor bit
LDFLAGS=-lcairo -lm -lX11
INCLUDE=-I/usr/include/cairo

# Define the deps
<<<<<<< HEAD
SOURCES=graphics/displayimage.c main.c
OBJECTS=$(SOURCES:.c=.o)
=======
SOURCES=main.cpp tree.cpp
OBJECTS=$(SOURCES:.cpp=.o)
>>>>>>> Changedd it to da refactor bit

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
<<<<<<< HEAD
	$(CC) $< -c -o $@ $(INCLUDE) $(CFLAGS)
=======
	$(CC) $< -o $@ $(INCLUDE) $(CFLAGS)
>>>>>>> Changedd it to da refactor bit
