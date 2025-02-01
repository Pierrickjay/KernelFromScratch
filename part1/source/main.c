#include "screen_manager.h"
#include "print_manager.h"



void main()
{
    terminal_buffer = (unsigned short*)VGA_ADDRESS;
    vga_index = 0;
    int nb;
    clear_screen();
    print_string("hello val ca va ? ", YELLOW);
    print_carriage_return();
    print_char('D', GREEN);
    print_carriage_return();
    print_number(8988, &nb, GREEN);
    print_carriage_return();
    print_k("ceci est un test ", 123);
    print_carriage_return();
    print_string("Versiwfwfefewfwefewoweewefwefew", RED);
    print_carriage_return();

}