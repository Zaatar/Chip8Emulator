#include "chip8.h"

chip8 myChip8;

int main(int argc, char **argv)
{
    // Setup render system and register input callbacks
    setupGraphics();
    setupInput();

    // Initialize the Chip8 system and load the game into the memory
    myChip8.initialize();
    myChip8.loadGame("pong");

    // emulation loop
    for(;;)
    {
        // emulate one cycle
        myChip8.emulateCycle();

        // if the draw flag is set, update the screen
        if(myChip8.drawing)
        {
            drawGraphics();
        }

        // store key press state (press and release)
        myChip8.setKeys();
    }

    return 0;
}