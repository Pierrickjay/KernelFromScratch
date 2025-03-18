#include "print_manager.h"
#include "screen_manager.h"

void print_42(void)
{
	set_color(&screen_context, BG(RED) | BLUE);
	print_string("              42424  42424242424\n");
	set_color(&screen_context, BG(BLUE) | GREEN);
	print_string("           42424     4242  42424\n");
	set_color(&screen_context, BG(GREEN) | RED);
	print_string("        42424        42    42424\n");
	set_color(&screen_context, BG(CYAN) | YELLOW);
	print_string("     42424               42424\n");
	set_color(&screen_context, BG(YELLOW) | MAGENTA);
	print_string("  42424                42424\n");
	set_color(&screen_context, BG(MAGENTA) | CYAN);
	print_string("4242424242424242424  42424    42\n");
	set_color(&screen_context, BG(BLUE) | GREEN | INTENSIVE);
	print_string("4242424242424242424  42424  4242\n");
	set_color(&screen_context, BG(WHITE) | RED | INTENSIVE);
	print_string("               4242  42424242424\n");
	set_color(&screen_context, BG(YELLOW) | YELLOW | INTENSIVE);
	print_string("               4242\n");
	set_color(&screen_context, BG(GREEN) | MAGENTA | INTENSIVE);
	print_string("               4242\n");
	set_color(&screen_context, BG(RED) | BLUE | INTENSIVE);
	print_string("               4242\n");
}

void main()
{
	init_screen_context(&screen_context);

	int nb = 8;
	clear_screen(&screen_context);
	set_color(&screen_context, YELLOW);
	print_string("hello val ca va ? ");
	set_color(&screen_context, GREEN);
	print_char('\n');
	print_char('D');
	print_char('\n');
	print_number(8988);
	set_color(&screen_context, WHITE);
	print_f("ceci est un int test %d", 123);
	print_char('\n');
	print_f("ceci est un char test %c", 'C');
	print_char('\n');
	int test = print_f("ceci est un string test %s", "frefjreferf");
	// print_f("ceci est un hex test %x", &nb);
	set_color(&screen_context, RED);
	print_string("Versiwfwfefewfwefewoweewefwefew\n");
	set_color(&screen_context, GREEN);
	print_number(test);

	kfs_write_colored_char(&screen_context, '\n', WHITE);
	print_42();
}
