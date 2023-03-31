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
    if (init_sdl()) exit(EXIT_FAILURE);

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

    display->renderer = SDL_CreateRenderer(display->window, -1, 0);

    SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 0);
    SDL_RenderClear(display->renderer);
    SDL_RenderPresent(display->renderer);
}

void update_display(Chip8 *chip8, Display *display) {
    int x, y;
    SDL_Rect rect = {.x = 0, .y = 0, .w = DISPLAY_SCALE, .h = DISPLAY_SCALE};

    SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 0);
    SDL_RenderClear(display->renderer);

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
}

void check_input(Chip8 *chip8) {
    SDL_Event event;
    printf("Checking for input\n");

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                printf("KEY PRESSED\n");
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: cleanup(); 
                        break;
                    case SDLK_1: 
                        chip8->keys[0x1] = 1; 
                        printf("1 PRESSED \n");
                        break;
                    case SDLK_2: 
                        chip8->keys[0x2] = 1; 
                        break;
                    case SDLK_3: chip8->keys[0x3] = 1; break;
                    case SDLK_4: chip8->keys[0xc] = 1; break;
                                 
                    case SDLK_q: chip8->keys[0x4] = 1; break;
                    case SDLK_w: chip8->keys[0x5] = 1; break;
                    case SDLK_e: chip8->keys[0x6] = 1; break;
                    case SDLK_r: chip8->keys[0xd] = 1; break;

                    case SDLK_a: chip8->keys[0x7] = 1; break;
                    case SDLK_s: chip8->keys[0x8] = 1; break;
                    case SDLK_d: chip8->keys[0x9] = 1; break;
                    case SDLK_f: chip8->keys[0xe] = 1; break;

                    case SDLK_z: chip8->keys[0xa] = 1; break;
                    case SDLK_x: chip8->keys[0x0] = 1; break;
                    case SDLK_c: chip8->keys[0xb] = 1; break;
                    case SDLK_v: chip8->keys[0xf] = 1; break;
                }

                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_1: chip8->keys[0x1] = 0; break;
                    case SDLK_2: chip8->keys[0x2] = 0; break;
                    case SDLK_3: chip8->keys[0x3] = 0; break;
                    case SDLK_4: chip8->keys[0xc] = 0; break;
                                 
                    case SDLK_q: chip8->keys[0x4] = 0; break;
                    case SDLK_w: chip8->keys[0x5] = 0; break;
                    case SDLK_e: chip8->keys[0x6] = 0; break;
                    case SDLK_r: chip8->keys[0xd] = 0; break;

                    case SDLK_a: chip8->keys[0x7] = 0; break;
                    case SDLK_s: chip8->keys[0x8] = 0; break;
                    case SDLK_d: chip8->keys[0x9] = 0; break;
                    case SDLK_f: chip8->keys[0xe] = 0; break;

                    case SDLK_z: chip8->keys[0xa] = 0; break;
                    case SDLK_x: chip8->keys[0x0] = 0; break;
                    case SDLK_c: chip8->keys[0xb] = 0; break;
                    case SDLK_v: chip8->keys[0xf] = 0; break;
                }

                break;
        }
    }
}
