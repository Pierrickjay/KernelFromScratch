#include "print_manager.h"
#include "screen_manager.h"

void print_42(void)
{
	print_string("              42424  42424242424\n", BG(RED | INTENSIVE) | BLUE);
	print_string("           42424     4242  42424\n", BG(BLUE | INTENSIVE) | GREEN);
	print_string("        42424        42    42424\n", BG(GREEN | INTENSIVE) | RED);
	print_string("     42424               42424\n", BG(CYAN | INTENSIVE) | YELLOW);
	print_string("  42424                42424\n", BG(YELLOW | INTENSIVE) | MAGENTA);
	print_string("4242424242424242424  42424    42\n", BG(MAGENTA | INTENSIVE) | CYAN);
	print_string("4242424242424242424  42424  4242\n", BG(BLUE | INTENSIVE) | GREEN | INTENSIVE);
	print_string("               4242  42424242424\n", BG(WHITE | INTENSIVE) | RED | INTENSIVE);
	print_string("               4242\n", BG(YELLOW | INTENSIVE) | YELLOW | INTENSIVE);
	print_string("               4242\n", BG(GREEN | INTENSIVE) | MAGENTA | INTENSIVE);
	print_string("               4242\n", BG(RED | INTENSIVE) | BLUE | INTENSIVE);
}

void main()
{
	init_screen_context(&screen_context);

	int nb = 8;
	clear_screen(&screen_context);
	print_string("hello val ca va ? ", YELLOW);
	kfs_write_char(&screen_context, '\n', WHITE);
	kfs_write_char(&screen_context, 'D', GREEN);
	kfs_write_char(&screen_context, '\n', WHITE);
	print_number(8988, GREEN);
	print_f("ceci est un int test %d", 123);
	kfs_write_char(&screen_context, '\n', WHITE);
	print_f("ceci est un char test %c", 'C');
	kfs_write_char(&screen_context, '\n', WHITE);
	int test = print_f("ceci est un string test %s", "frefjreferf");
	// print_f("ceci est un hex test %x", &nb);
	print_string("Versiwfwfefewfwefewoweewefwefew\n", RED);
	print_number(test, GREEN);

	kfs_write_char(&screen_context, '\n', WHITE);
	print_42();
}
