#include "chip8.h"

void Chip8::initialize()
{
    // Initialize registers and memory once
    pc = 0x200;     // Program counter starts at 0x200
    opcode = 0;     // Reset current opcode
    I = 0;          // reset Index register
    sp = 0;         // reset stack pointer

    // clear display
    memset(gfx, 0x00, sizeof(gfx));
    // clear stack
    memset(stack, 0, sizeof(stack));
    // clear registers v0-vf
    memset(V, 0x00, sizeof(V));
    // clear memory
    memset(memory, 0x00, sizeof(memory));

    // Load fontset
    for(int i = 0; i < 80; ++i)
    {
        memory[i] = chip8_fontset[i];
    }

    // reset timers
    delay_timer = 3600;
    sound_timer = 3600;

}

void Chip8::loadGame(unsigned char inGame)
{
    for(int i = 0; i < bufferSize; ++i)
    {
        // in Chip 8 programs are loaded starting from 0x200 address which is equal to 512
        memory[i+512] = buffer[i];
    }
}

void Chip8::emulateCycle()
{
    // Fetch opcode
    opcode = memory[pc] << 8 | memory[pc+1];

    // Decode opcode
    switch(opcode & 0xF000)
    {
        // Execute opcode
        case 0xA000: // ANNN: Sets I to the address NNN
            I = opcode & 0x0FFF;
            pc += 2;
            break;

        case 0x2000: // 2NNN: Temporary jump to address NNN
            stack[sp] = pc;
            ++sp;
            pc = opcode & 0x0FFF;
            break;

        case 0x0000:
            switch(opcode & 0x000F)
            {
                case 0x0000: // 0x00E0: Clears the screen
                    //Execute opcode
                    break;

                case 0x000E: // 0x00EE: Returns from subroutine
                    //Execute opcode
                    break;

                default:
                    printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
            }

        case 0x8000:
            switch(opcode & 0x000F)
            {
                case 0x0004: // adds value of VY to VX, Register VF set to 1 if there is a carry, 0 otherwise
                    if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
                        V[0xF] = 1;
                    else
                        V[0xF] = 0;
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;

                default:
                    printf("Unknown opcode [0x8000]: 0x%X\n", opcode);
            }

        case 0xF000:
            switch(opcode & 0x00FF)
            {
                case 0x0007:
                    break;

                case 0x000A:
                    break;

                case 0x0015:
                    break;

                case 0x0018:
                    break;

                case 0x001E:
                    break;

                case 0x0029:
                    break;

                case 0x0033: // stores the binary coded decimal representation of VX at I, I+1 and I + 2 
                    I = V[(opcode & 0xF00) >> 8] / 100;
                    I + 1 = (V[(opcode & 0xF00) >> 8] /10) % 10;
                    I + 2 = (V[(opcode & 0xF00) >> 8] % 100) % 10;
                    pc += 2;
                    break;

                case 0x0055:
                    break;

                case 0x0065:
                    break;

                default:
                    printf("Unknown opcode [0xF000]: 0x%X\n", opcode);
            }

        
        default:
            printf("Unknown opcode: 0x%X\n", opcode);
    }

    // Update timers
    if(delay_timer > 0)
    {
        --delay_timer;
    }

    if(sound_timer > 0)
    {
        if(sound_timer == 1)
        {
            printf("BEEP!\n");
            --sound_timer;
        }
    }
}