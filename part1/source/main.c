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
    printf("ceci est un int test %d", 123);
    print_carriage_return();
    printf("ceci est un char test %c", 'C');
    print_carriage_return();
    int test = printf("ceci est un string test %s", "frefjreferf");
    // printf("ceci est un hex test %x",); 
    print_string("Versiwfwfefewfwefewoweewefwefew\n", RED);
    print_number(test, &nb,GREEN);
}