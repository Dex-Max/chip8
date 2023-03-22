CC = gcc
CFLAGS = -ansi -Wall

# Compiler and linker options for SDL2
SDL_CFLAGS= $(shell sdl2-config --cflags)
SDL_LFLAGS= $(shell sdl2-config --libs)

override CFLAGS += $(SDL_CFLAGS)

all: chip8

chip8: chip8.o display.o
	$(CC) chip8.o display.o $(SDL_LFLAGS) -o chip8

chip8.o: chip8.c chip8.h
	$(CC) $(CFLAGS) chip8.c -c chip8.o

display.o: display.c display.h
	$(CC) $(CFLAGS) display.c -c display.o

clean:
	rm *.o
