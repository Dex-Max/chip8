#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#define MEM_SIZE 4096
#define STACK_SIZE 16
#define NUM_REGISTERS 16

typedef struct {
    uint8_t mem[MEM_SIZE];
    uint16_t stack[STACK_SIZE];
    uint8_t sp;

    uint8_t reg[NUM_REGISTERS];
    uint16_t I_reg;

    uint16_t pc;
    uint16_t op;

    uint8_t display[64 * 32];

    uint8_t dt;
    uint8_t st;

    uint8_t keys[16];
} Chip8;

#endif
