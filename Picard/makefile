CC=gcc
CFLAGS=-Wall -Wextra
LDFLAGS=
SOURCES=main.c io.c dades.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=executable

all: $(SOURCES) $(EXECUTABLE)
	rm -f *.o *~

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.o:
	$(CC) $(CFLAGS) $< -o $@
