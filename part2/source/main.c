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
	print_string("hello val ca va ? ", YELLOW);
	print_carriage_return();
	print_char('D', GREEN);
	print_carriage_return();
	print_number(8988, GREEN);
	print_f("ceci est un int test %d", 123);
	print_carriage_return();
	print_f("ceci est un char test %c", 'C');
	print_carriage_return();
	int test = print_f("ceci est un string test %s", "frefjreferf");
	print_number(test, GREEN);
	print_carriage_return();
	// print_f("ceci est un hex test %x", &nb);
	print_string("Versiwfwfefewfwefewoweewefwefew\n", RED);
}

void print_k_test()
{
	printk(KERN_EMERG "System is about to crash!\n");
	printk(KERN_ALERT "Immediate action required!\n");
	printk(KERN_CRIT "Critical condition detected\n");
	printk(KERN_ERR "An error occurred: %d\n", -1);
	printk(KERN_WARNING "Warning: low memory\n");
	printk(KERN_NOTICE "System initialization complete\n");
	printk(KERN_INFO "Starting process with PID %d\n", 12912);
	printk(KERN_DEBUG "Debug info: value = %x\n", 0x1234ABCD);
}

void main()
{
	clear_screen();

	terminal_buffer = (unsigned short *)VGA_ADDRESS;
	vga_index		= 0;

	test_print();
	print_42();
	print_k_test();
}
