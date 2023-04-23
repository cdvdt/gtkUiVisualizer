CC = gcc
CFLAGS = $(shell pkg-config --cflags gtk4 )
LDFLAGS = $(shell pkg-config --libs gtk4 )

all: main.o resources.o deps
	gcc -o build/gtkUiVisualizer src/ui/generated/resources.o src/view/mainwindow.o src/view/viewedwindow.o src/main.o $(LDFLAGS)

main.o: ./src/main.o

deps: ./src/view/mainwindow.o ./src/view/viewedwindow.o

resources:
	glib-compile-resources resouces.xml --generate --target=src/ui/generated/resources.c
	glib-compile-resources resouces.xml --generate --target=src/ui/generated/resources.h

resources.o: resources ./src/ui/generated/resources.o

clean:
	rm -rf src/**/*.o main
	rm -f src/ui/generated/*

.PHONY: all clean

