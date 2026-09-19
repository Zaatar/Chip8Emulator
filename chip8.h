#ifndef CHIP8_H_
#define CHIP8_H_
struct Chip8
{
    void initialize();
    void loadGame(unsigned char inGame);
    void emulateCycle();
    // simple flag to signal if the CHIP-8 is running or not
    unsigned char running;

    // op codes being 2 bytes long, an unsigned short is used to store them
    unsigned short opcode;

    // chip8 memory being 4K in size that translates to 4096
    // 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
    // 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
    // 0x200-0xFFF - Program ROM and work RAM 
    unsigned char memory[4096];

    // 16 Registers
    unsigned char V[16];

    // Index Register
    unsigned short I;

    // Program counter
    unsigned short pc;

    // graphics of chip8 - 2048 pixels (64x32)
    unsigned char gfx[64*32];

    // flag to indicate if drawing in this frame
    unsigned char drawing;

    // two timer registers that count at 60 hz, when set above zero they will count down to zero
    unsigned char delay_timer;
    unsigned char sound_timer;

    // 16 levels of stack to perform jumps or call subroutines
    unsigned short stack[16];
    // a stack pointer to indicate the pointer to which the system should return after a subroutine
    unsigned short sp;

    // key pad of the chip 8 going from 0x0 - 0xF
    unsigned char key[16];

    // chip 8 fontset
    unsigned char chip8_fontset[80] =
    { 
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
};

typedef struct Chip8 Chip8;
#endif