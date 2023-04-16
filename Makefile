CC = gcc
CFLAGS = $(shell pkg-config --cflags gtk4 )
LDFLAGS = $(shell pkg-config --libs gtk4 )

all: main.o copy
	gcc -o build/gtkUiVisualizer src/main.o $(LDFLAGS)

main.o: ./src/main.o

copy:
	cp src/ui/* build/ui/

clean:
	rm -f src/*.o main

.PHONY: all clean