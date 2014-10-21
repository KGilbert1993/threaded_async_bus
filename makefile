# Default makefile 
# 30 August 2014
# Kevin Gilbert
# EE460N

CC=g++
CFLAGS=-Wall -o -g
LDFLAGS=-pthread -std=c++11
SOURCES=asynch_bus.cpp
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=out

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CLFAGS) $< -o $@

clean: 
	rm -rf *o $(EXECUTABLE)
