#include <stdio.h>
#include <stdlib.h>
#include "display.h"
#include "chip8.h"

void init_chip8(Chip8 *chip8) {
    memset(chip8->stack, 0, 16 * sizeof(uint16_t));
    chip8->sp = 0;
    memset(chip8->mem, 0, 4096);
    memset(chip8->reg, 0, 16);
    chip8->I_reg = 0;
    chip8->pc = 0x200;
}

void load_program(Chip8 *chip8, char filename[]) {
    FILE *f = fopen(filename, "r");
    int size;
    
    if (f != NULL) {
        fseek(f, 0, SEEK_END);
        size = ftell(f);
        fseek(f, 0, SEEK_SET);
        
        if (size <= 0xfff - 0x200) {
            fread(&(chip8->mem[512]), 1, size, f);
            fclose(f);
        } else {
            printf("Program too large\n");
            exit(EXIT_FAILURE);
        }
    }
}

void execute_opcode(Chip8 *chip8, Display *display) {
    switch (chip8->op >> 12) {
        case 0:
            if (chip8->op == 0x00e0) {
                memset(chip8->display, 0, 64 * 32);
            }

            break;
        case 1:
            chip8->pc = chip8->op & 0xfff; 
            break;
        case 6:
            chip8->reg[(chip8->op >> 8) & 0xf] = chip8->op & 0xff;
            break;
        case 7:
            chip8->reg[(chip8->op >> 8) & 0xf] += chip8->op & 0xff;
            break;
        case 0xA:
            chip8->I_reg = chip8->op & 0xfff; 
            break;
        case 0xD:
            int x = chip8->reg[(chip8->op >> 8) & 0xf] % 64;
            int y = chip8->reg[(chip8->op >> 4) & 0xf] % 32; 
            int n = chip8->op & 0xf;
            int i;

            for (i = 0; i < n; i++) {
                uint8_t sprite = chip8->mem[chip8->I_reg + i];
                int p;
                for (p = 0; p < 8; p++) {
                    if ((y + i) * 64 + x + p < 64 * 32) {
                        chip8->display[(y + i) * 64 + (x + p)] ^= (sprite >> (7 - p)) & 1;
                    }
                }
            }

            update_display(chip8, display);

            break;
    }
}

int main(int argc, char *argv[]) {
    Chip8 chip8;
    Display display;

    init_chip8(&chip8);
    init_display(&display);

    load_program(&chip8, argv[1]);

    while (chip8.pc < 4096) {
        chip8.op = (chip8.mem[chip8.pc] << 8) | chip8.mem[chip8.pc + 1];
        execute_opcode(&chip8, &display);
        chip8.pc += 2; 
    }

    while(1) {}

    cleanup();

    return 0;
}
