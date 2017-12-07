CC=gcc
CFLAGS=-Wall -Wextra
LDFLAGS=-lpthread
SOURCES=main.c io.c dades.c communication.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=executable

all: $(SOURCES) $(EXECUTABLE)
	rm -f *.o *~

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@  $(LDFLAGS)

.o:
	$(CC) $(CFLAGS) $< -o $@
