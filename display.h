#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include "chip8.h"

typedef struct {
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Renderer *renderer;
} Display;

int init_sdl();
void init_display(Display *display);
void update_display(Chip8 *chip8, Display *display);
void cleanup();

#endif
