#include "screen_manager.h"
#include "print_manager.h"


void next_line(void)
{
    vga_index += L_WINDOW - (vga_index % L_WINDOW);
}

void print_42(void)
{
    print_string("              42424  42424242424", BG(RED | INTENSIVE) | BLUE);
    next_line();
    print_string("           42424     4242  42424", BG(BLUE | INTENSIVE) | GREEN);
    next_line();
    print_string("        42424        42    42424", BG(GREEN | INTENSIVE) | RED);
    next_line();
    print_string("     42424               42424", BG(CYAN | INTENSIVE) | YELLOW);
    next_line();
    print_string("  42424                42424", BG(YELLOW | INTENSIVE) | MAGENTA);
    next_line();
    print_string("4242424242424242424  42424    42", BG(MAGENTA | INTENSIVE) | CYAN);
    next_line();
    print_string("4242424242424242424  42424  4242", BG(BLUE | INTENSIVE) | GREEN | INTENSIVE);
    next_line();
    print_string("               4242  42424242424", BG(WHITE | INTENSIVE) | RED | INTENSIVE);
    next_line();
    print_string("               4242", BG(YELLOW | INTENSIVE) | YELLOW | INTENSIVE);
    next_line();
    print_string("               4242", BG(GREEN | INTENSIVE) | MAGENTA | INTENSIVE);
    next_line();
    print_string("               4242", BG(RED | INTENSIVE) | BLUE | INTENSIVE);
}

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

    print_42();
}