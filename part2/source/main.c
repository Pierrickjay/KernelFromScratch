#include "gdt.h"
#include "print_manager.h"
#include "screen_manager.h"

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

void test_print()
{
	int nb = 8;
	print_string("hello val ca va ? \n");
	kfs_write_char(&screen_context,'D');
	print_f("\n");
	print_number(8988);
	print_f("ceci est un int test %d\n", 123);
	print_f("\n");
	print_f("ceci est un char test %c\n", 'C');
	print_f("\n");
	int test = print_f("ceci est un string test %s\n", "frefjreferf");
	print_number(test);
	print_f("\n");
	// print_f("ceci est un hex test %x", &nb);
	print_string("Versiwfwfefewfwefewoweewefwefew\n");
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

void main()
{
	clear_screen();

	terminal_buffer = (unsigned short *)VGA_ADDRESS;
	vga_index		= 0;

	// test_print();
	// print_42();
	// print_k_test();
	// clear_screen();
	print_gdt_summary();
}
