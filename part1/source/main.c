#include "screen_manager.h"


void main()
{
    terminal_buffer = (unsigned short*)VGA_ADDRESS;
    vga_index = 0;

    clear_screen();
    print_string("hello val ca va ? ", YELLOW);
    vga_index = 80;
    print_string("Versiwfwfefewfwefewoweewefwefew", RED);
    vga_index = 160;
}