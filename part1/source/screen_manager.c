
#include "screen_manager.h"

unsigned short* terminal_buffer;
unsigned int vga_index;

void clear_screen(void)
{
    for (int index = 0; index < H_WINDOW * L_WINDOW * 2; index += 2) // trouver pourquoi 2
    {
        terminal_buffer[index] = ' ';
    }
}
