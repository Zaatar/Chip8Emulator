#include <stdio.h>
#include <string.h>
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
    delay_timer = 0;
    sound_timer = 0;

    drawing = false;
    running = true;
}

/*void Chip8::loadGame(unsigned char inGame)
{
    for(int i = 0; i < bufferSize; ++i)
    {
        // in Chip 8 programs are loaded starting from 0x200 address which is equal to 512
        memory[i+512] = buffer[i];
    }
}*/

void Chip8::emulateCycle()
{
    // Fetch opcode
    opcode = memory[pc] << 8 | memory[pc+1];

    // Decode opcode
    switch(opcode & 0xF000)
    {
        case 0xA000: // ANNN: Sets I to the address NNN
            I = opcode & 0x0FFF;
            pc += 2;
            break;
        
        case 0x1000: // 1NNN: Jump address, jump to location NNN
            pc = opcode & 0x0FFF;
            break;

        case 0x2000: // 2NNN: Temporary jump to address NNN
            stack[sp] = pc + 2;
            ++sp;
            pc = opcode & 0x0FFF;
            break;

        case 0x6000: // 6XNN: LD Vx, byte, The interpreter puts the value NN into register Vx
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc += 2;
            break;
        
        case 0x7000: 
        // 7XNN: Add Vx, byte. The interpreter adds the value NN to the value of register 
        // Vx then stores it in Vx
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc += 2;
            break;

        case 0xD000:
        {
            unsigned short x = V[(opcode & 0x0F00) >> 8];
            unsigned short y = V[(opcode & 0x00F0) >> 4];
            unsigned short height = opcode & 0x000F;
            unsigned short pixel;

            V[0xF] = 0;
            for (int yLine = 0; yLine < height; yLine++)
            {
                pixel = memory[I + yLine];
                for(int xLine = 0; xLine < 8; xLine++)
                {
                    // don't quite understand the below line need to check further
                    if((pixel & (0x80 >> xLine)) != 0)
                    {
                        if(gfx[(x + xLine + ((y + yLine) * 64))] == 1)
                            V[0xF] = 1;

                        gfx[x + xLine + ((y + yLine) * 64)] ^= 1;
                    }
                }
            }

            drawing = true;
            pc += 2;
        }
        break;

        case 0x0000:
            switch(opcode & 0x000F)
            {
                case 0x0000: // 0x00E0: Clears the screen
                    memset(gfx, 0x00, sizeof(gfx));
                    drawing = true;
                    pc += 2;
                    break;

                case 0x000E: // 0x00EE: Returns from subroutine
                    if(sp > 0)
                    {
                        sp--;
                        pc = stack[sp];
                    }
                    else
                    {
                        printf("opcode [0x00EE]: 0x%X, sp <= 0", opcode);
                    }
                        
                    break;

                default:
                    printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
            }
            break;

        case 0x8000:
            switch(opcode & 0x000F)
            {
                case 0x0004: // 8XY4: adds value of VY to VX, Register VF set to 1 if there is a carry, 0 otherwise
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
            break;

        case 0xE000:
            switch(opcode & 0x00FF)
            {
                case 0x009E: // EX9E: Skips the next instruction if the key stored in VX is pressed
                    if(key[V[(opcode & 0x0F00) >> 8]] != 0)
                        pc += 4;
                    else
                        pc += 2;
                    break;

                default:
                    printf("Unknown opcode [0xE000]: 0x%X\n", opcode);
            }
            break;


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

                case 0x0033: // FX33: stores binary coded decimal representation of VX at I, I+1 and I + 2 
                    memory[I] = V[(opcode & 0xF00) >> 8] / 100;
                    memory[I + 1] = (V[(opcode & 0xF00) >> 8] /10) % 10;
                    memory[I + 2] = (V[(opcode & 0xF00) >> 8] % 100) % 10;
                    pc += 2;
                    break;

                case 0x0055:
                    break;

                case 0x0065:
                    break;

                default:
                    printf("Unknown opcode [0xF000]: 0x%X\n", opcode);
            }
            break;

        
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
            printf("BEEP!\n");

        --sound_timer;
    }
}