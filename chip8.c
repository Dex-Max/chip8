#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "display.h"
#include "chip8.h"

void init_chip8(Chip8 *chip8) {
    uint8_t font[] = {
        0xf0, 0x90, 0x90, 0x90, 0xf0,
        0x20, 0x60, 0x20, 0x20, 0x70,
        0xf0, 0x10, 0xf0, 0x80, 0xf0,
        0xf0, 0x10, 0xf0, 0x10, 0xf0,
        0x90, 0x90, 0xf0, 0x10, 0x10,
        0xf0, 0x80, 0xf0, 0x10, 0xf0,
        0xf0, 0x80, 0xf0, 0x90, 0xf0,
        0xf0, 0x10, 0x20, 0x40, 0x40,
        0xf0, 0x90, 0xf0, 0x90, 0xf0,
        0xf0, 0x90, 0xf0, 0x10, 0xf0,
        0xf0, 0x90, 0xf0, 0x90, 0x90,
        0xe0, 0x90, 0xe0, 0x90, 0xe0,
        0xf0, 0x80, 0x80, 0x80, 0xf0,
        0xe0, 0x90, 0x90, 0x90, 0xe0,
        0xf0, 0x80, 0xf0, 0x80, 0xf0,
        0xf0, 0x80, 0xf0, 0x80, 0x80
    };
    
    memcpy(chip8->mem, font, sizeof(font));
    memset(chip8->stack, 0, 16 * sizeof(uint16_t));
    chip8->sp = 0;
    memset(chip8->mem, 0, 4096);
    memset(chip8->reg, 0, 16);
    chip8->I_reg = 0;
    chip8->pc = 0x200;
    memset(chip8->display, 0, 64 * 32);
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
            switch (chip8->op & 0xff) {
                case 0xe0:
                    memset(chip8->display, 0, 64 * 32);
                    chip8->pc += 2;
                    break;
                case 0xee:
                    chip8->sp -= 1;
                    chip8->pc = chip8->stack[chip8->sp] + 2;
                    break;
            }

            break;
        case 1:
            chip8->pc = chip8->op & 0xfff; 
            break;
        case 2:
            chip8->stack[chip8->sp] = chip8->pc;
            chip8->sp += 1;
            chip8->pc = chip8->op & 0xfff;
            break;
        case 3:
            if (chip8->reg[(chip8->op >> 8) & 0xf] == (chip8->op & 0xff)) chip8->pc += 2;
            chip8->pc += 2;
            break;
        case 4:
            if (chip8->reg[(chip8->op >> 8) & 0xf] != (chip8->op & 0xff)) chip8->pc += 2;
            chip8->pc += 2;
            break;
        case 5:
            if (chip8->reg[(chip8->op >> 8) & 0xf] == chip8->reg[(chip8->op >> 4) & 0xf]) chip8->pc += 2;
            chip8->pc += 2;
            break;
        case 6:
            chip8->reg[(chip8->op >> 8) & 0xf] = chip8->op & 0xff;
            chip8->pc += 2;
            break;
        case 7:
            chip8->reg[(chip8->op >> 8) & 0xf] += chip8->op & 0xff;
            chip8->pc += 2;
            break;
        case 8:
            int reg_x = (chip8->op >> 8) & 0xf;
            int reg_y = (chip8->op >> 4) & 0xf;

            switch (chip8->op & 0xf) {
                case 0:
                    chip8->reg[reg_x] = chip8->reg[reg_y];
                    break;
                case 1:
                    chip8->reg[reg_x] = chip8->reg[reg_x] | chip8->reg[reg_y];
                    break;
                case 2:
                    chip8->reg[reg_x] = chip8->reg[reg_x] & chip8->reg[reg_y];
                    break;
                case 3:
                    chip8->reg[reg_x] = chip8->reg[reg_x] ^ chip8->reg[reg_y];
                    break;
                case 4:
                    uint8_t add = chip8->reg[reg_x] + chip8->reg[reg_y];

                    if (add < chip8->reg[reg_x]) {
                        chip8->reg[0xf] = 1;
                    } else {
                        chip8->reg[0xf] = 0;
                    }

                    chip8->reg[reg_x] = add;
                    break;
                case 5:
                    uint8_t sub = chip8->reg[reg_x] - chip8->reg[reg_y];

                    if (chip8->reg[reg_y] > chip8->reg[reg_x]) {
                        chip8->reg[0xf] = 1;
                    } else {
                        chip8->reg[0xf] = 0;
                    }

                    chip8->reg[reg_x] = sub;
                    break;
                case 6:
                    chip8->reg[0xf] = chip8->reg[reg_x] & 1;
                    chip8->reg[reg_x] >>= 1;
                    break;
                case 7:
                    uint8_t subn = chip8->reg[reg_y] - chip8->reg[reg_x];

                    if (chip8->reg[reg_y] > chip8->reg[reg_x]) {
                        chip8->reg[0xf] = 1;
                    } else {
                        chip8->reg[0xf] = 0;
                    }

                    chip8->reg[reg_x] = subn;
                    break;
                case 0xE:
                    chip8->reg[0xf] = chip8->reg[reg_x] >> 7;
                    chip8->reg[reg_x] <<= 1;
                    break;
            }

            chip8->pc += 2;
            break;
        case 9:
            if (chip8->reg[(chip8->op >> 8) & 0xf] != chip8->reg[(chip8->op >> 4) & 0xf]) chip8->pc += 2;
            chip8->pc += 2;
            break;
        case 0xA:
            chip8->I_reg = chip8->op & 0xfff; 
            chip8->pc += 2;
            break;
        case 0xB:
            chip8->pc = (chip8->op & 0xfff) + chip8->reg[0];
            break;
        case 0xC:
            chip8->reg[(chip8->op >> 4) & 0xf] = (rand() % 0xff) & (chip8->op & 0xff);
            chip8->pc += 2;
            break;
        case 0xD:
            int x = chip8->reg[(chip8->op >> 8) & 0xf] % 64;
            int y = chip8->reg[(chip8->op >> 4) & 0xf] % 32; 
            int n = chip8->op & 0xf;
            int i;

            chip8->reg[0xf] = 0;

            for (i = 0; i < n; i++) {
                uint8_t sprite = chip8->mem[chip8->I_reg + i];
                int p;

                for (p = 0; p < 8; p++) {
                    uint8_t pixel_index = (y + i) * 64 + (x + p);

                    if (pixel_index < 64 * 32) {
                        if (chip8->display[pixel_index] && (sprite >> (7 - p)) & 1) {
                            chip8->reg[0xf] = 1;
                        }

                        chip8->display[(y + i) * 64 + (x + p)] ^= (sprite >> (7 - p)) & 1;
                    }
                }
            }

            update_display(chip8, display);
            chip8->pc += 2;

            break;
        default:
            printf("Invalid opcode");
            chip8->pc += 2;
    }
}

void debug(Chip8 *chip8) {
    int i;
    printf("**********************************\n");
    printf("Program counter: %x\n", chip8->pc);
    printf("Current op: %x\n", chip8->op);
    printf("Registers:\n");
    for (i = 0; i < 16; i++) {
        printf("V%x: %x | ", i, chip8->reg[i]);
    }
    printf("I reg: %x\n", chip8->I_reg);
    printf("Stack pointer: %x\n", chip8->sp);
    printf("Stack: ");
    for (i = 0; i < 16; i++) {
        printf("[%d]: %x  |  ", i, chip8->stack[i]);
    }
    printf("\n**********************************\n");
}

int main(int argc, char *argv[]) {
    Chip8 chip8;
    Display display;
    
    srand(time(NULL));

    init_chip8(&chip8);
    init_display(&display);

    load_program(&chip8, argv[1]);

    while (chip8.pc < 4096) {
        getchar();
        debug(&chip8);
        chip8.op = (chip8.mem[chip8.pc] << 8) | chip8.mem[chip8.pc + 1];
        execute_opcode(&chip8, &display);
    }

    cleanup();

    return 0;
}
