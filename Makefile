CC=g++
CFLAGS=-c -Wall -Werror -O2 #-pedantic
SOURCES=main.cpp huffman.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=arj

all: $(SOURCES) $(EXECUTABLE)
	mkdir bin; mv arj bin/arj
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -rf *.o  *~ ./bin
