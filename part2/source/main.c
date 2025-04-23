#include "print_manager.h"
#include "screen_manager.h"
#include "gdt.h"

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

void print_k_test()
{
	print_k(KERN_EMERG "System is about to crash!\n");
	print_k(KERN_ALERT "Immediate action required!\n");
	print_k(KERN_CRIT "Critical condition detected\n");
	print_k(KERN_ERR "An error occurred: %d\n", -1);
	print_k(KERN_WARNING "Warning: low memory\n");
	print_k(KERN_NOTICE "System initialization complete\n");
	print_k(KERN_INFO "Starting process with PID %d\n", 12912);
	print_k(KERN_DEBUG "Debug info: value = %x\n", 0x1234ABCD);
}

void print_test()
{
	// clear_screen(&screen_context);
	print_string("hello val ca va ?\n");
	kfs_write_char(&screen_context, 'D');
	kfs_write_char(&screen_context, '\n');
	print_number(8988);
	kfs_write_char(&screen_context, '\n');
}

void print_f_test()
{
	int nb			= 8;
	// clear_screen(&screen_context);
	print_f("ceci est un int test %d\n", 123);
	print_f("ceci est un char test %c\n", 'C');
	int test = print_f("ceci est un string test %s\n", "frefjreferf");
	print_f("ceci est un hex test %x\n", &nb);
	print_string("Versiwfwfefewfwefewoweewefwefew\n");
	print_number(test);
}

void main()
{
	gdt_install();
	init_screen_context(&screen_context);

	int nb = 8;
	// clear_screen(&screen_context);
	set_color(&screen_context, BG(BLACK) | WHITE);
	// print_test();
	// print_k_test();
	// print_f_test();
	// print_42();

	// kfs_write_char(&screen_context, 'a');
	// print_f("\n%d-%d\n", screen_context.desktops[0].cursor.x, screen_context.desktops[0].cursor.y);
	print_gdt_summary();
}