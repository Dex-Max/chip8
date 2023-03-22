#include "display.h"
#include "chip8.h"

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SCALE 10

int init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        printf("SDL init failure: %s\n", SDL_GetError());

        return 1;
    }
    
    return 0;
}

void init_display(Display *display) {
    printf("inside init_display\n");

    if (init_sdl()) exit(EXIT_FAILURE);

    printf("init_sdl success\n");

    display->window = SDL_CreateWindow(
        "Chip8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DISPLAY_WIDTH * DISPLAY_SCALE,
        DISPLAY_HEIGHT * DISPLAY_SCALE, 
        0
    );

    if (display->window == NULL) {
        printf("Failed to create window\n");
        exit(EXIT_FAILURE);
    }

    printf("created window\n");
    display->renderer = SDL_CreateRenderer(display->window, -1, 0);
    printf("created renderer\n");

    SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 1);
    SDL_RenderClear(display->renderer);
    SDL_RenderPresent(display->renderer);
}

void update_display(Chip8 *chip8, Display *display) {
    int x, y;
    SDL_Rect rect = {.x = 0, .y = 0, .w = DISPLAY_SCALE, .h = DISPLAY_SCALE};

    for (y = 0; y < DISPLAY_HEIGHT; y++) {
        for (x = 0; x < DISPLAY_WIDTH; x++) {
            if (chip8->display[y * 64 + x]) {
                rect.x = x * DISPLAY_SCALE;
                rect.y = y * DISPLAY_SCALE;

                SDL_SetRenderDrawColor(display->renderer, 255, 255, 255, 1);
                SDL_RenderFillRect(display->renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(display->renderer);
}

void cleanup() {
    SDL_Quit();
    return 0;
}
