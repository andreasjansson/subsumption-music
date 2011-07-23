CC=gcc
CFLAGS=-Wall -g -O4
LIBS=-lm -lconfig
SOURCES=source.c synth.c agent.c system.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=source

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LIBS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
