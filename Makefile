CC=g++
CFLAGS=-c -Wall -Werror -pedantic -O3
SOURCES=main.cpp huffman.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=arj

all: $(SOURCES) $(EXECUTABLE)
	mkdir -p bin; mv arj bin/arj

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -rf *.o  *~ ./bin
