#include "print_manager.h"

void print_42(void)
{
	unsigned char color = get_color(&screen_context);
	print_string("              ");
	set_color(&screen_context, BG(RED) | BLUE);
	print_string("42424");
	set_color(&screen_context, color);
	print_string("  ");
	set_color(&screen_context, BG(RED) | BLUE);
	print_string("42424242424\n");

	set_color(&screen_context, color);
	print_string("           ");
	set_color(&screen_context, BG(BLUE) | GREEN);
	print_string("42424");
	set_color(&screen_context, color);
	print_string("     ");
	set_color(&screen_context, BG(BLUE) | GREEN);
	print_string("4242");
	set_color(&screen_context, color);
	print_string("  ");
	set_color(&screen_context, BG(BLUE) | GREEN);
	print_string("42424\n");

	// set_color(&screen_context, BG(GREEN) | RED);
	// "        "42424"        "42"    "42424\n"
	set_color(&screen_context, color);
	print_string("        ");
	set_color(&screen_context, BG(GREEN) | RED);
	print_string("42424");
	set_color(&screen_context, color);
	print_string("        ");
	set_color(&screen_context, BG(GREEN) | RED);
	print_string("42");
	set_color(&screen_context, color);
	print_string("    ");
	set_color(&screen_context, BG(GREEN) | RED);
	print_string("42424\n");

	// set_color(&screen_context, BG(CYAN) | YELLOW);
	// "     "42424"               "42424\n"
	set_color(&screen_context, color);
	print_string("     ");
	set_color(&screen_context, BG(CYAN) | YELLOW);
	print_string("42424");
	set_color(&screen_context, color);
	print_string("               ");
	set_color(&screen_context, BG(CYAN) | YELLOW);
	print_string("42424\n");

	// set_color(&screen_context, BG(YELLOW) | MAGENTA);
	// "  "42424"                "42424\n"
	set_color(&screen_context, color);
	print_string("  ");
	set_color(&screen_context, BG(YELLOW) | MAGENTA);
	print_string("42424");
	set_color(&screen_context, color);
	print_string("                ");
	set_color(&screen_context, BG(YELLOW) | MAGENTA);
	print_string("42424\n");

	// set_color(&screen_context, BG(MAGENTA) | CYAN);
	// "4242424242424242424"  "42424"    "42\n"
	set_color(&screen_context, BG(MAGENTA) | CYAN);
	print_string("4242424242424242424");
	set_color(&screen_context, color);
	print_string("  ");
	set_color(&screen_context, BG(MAGENTA) | CYAN);
	print_string("42424");
	set_color(&screen_context, color);
	print_string("    ");
	set_color(&screen_context, BG(MAGENTA) | CYAN);
	print_string("42\n");

	// set_color(&screen_context, BG(BLUE) | GREEN | INTENSIVE);
	// "4242424242424242424"  "42424"  "4242\n"
	set_color(&screen_context, BG(BLUE) | GREEN | INTENSIVE);
	print_string("4242424242424242424");
	set_color(&screen_context, color);
	print_string("  ");
	set_color(&screen_context, BG(BLUE) | GREEN | INTENSIVE);
	print_string("42424");
	set_color(&screen_context, color);
	print_string("  ");
	set_color(&screen_context, BG(BLUE) | GREEN | INTENSIVE);
	print_string("4242\n");

	// set_color(&screen_context, BG(WHITE) | RED | INTENSIVE);
	// "               "4242"  "42424242424\n"
	set_color(&screen_context, color);
	print_string("               ");
	set_color(&screen_context, BG(WHITE) | RED | INTENSIVE);
	print_string("4242");
	set_color(&screen_context, color);
	print_string("  ");
	set_color(&screen_context, BG(WHITE) | RED | INTENSIVE);
	print_string("42424242424\n");

	// set_color(&screen_context, BG(YELLOW) | YELLOW | INTENSIVE);
	// "               "4242\n"
	set_color(&screen_context, color);
	print_string("               ");
	set_color(&screen_context, BG(YELLOW) | YELLOW | INTENSIVE);
	print_string("4242\n");

	// set_color(&screen_context, BG(GREEN) | MAGENTA | INTENSIVE);
	// "               "4242\n"
	set_color(&screen_context, color);
	print_string("               ");
	set_color(&screen_context, BG(GREEN) | MAGENTA | INTENSIVE);
	print_string("4242\n");

	// set_color(&screen_context, BG(RED) | BLUE | INTENSIVE);
	// "               "4242\n"
	set_color(&screen_context, color);
	print_string("               ");
	set_color(&screen_context, BG(RED) | BLUE | INTENSIVE);
	print_string("4242\n");

	set_color(&screen_context, color);
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
	clear_screen(&screen_context);
	print_string("hello val ca va ?\n");
	kfs_write_char(&screen_context, 'D');
	kfs_write_char(&screen_context, '\n');
	print_number(8988);
	kfs_write_char(&screen_context, '\n');
}

void print_f_test()
{
	int nb = 8;
	print_f("ceci est un int test %d\n", 123);
	print_f("ceci est un char test %c\n", 'C');
	int test = print_f("ceci est un string test %s\n", "frefjreferf");
	int writed_char = print_f("ceci est un hex test %x\n", &nb);
	print_f("nb of writed char: %d\n", writed_char);
	print_string("Versiwfwfefewfwefewoweewefwefew\n");
	print_number(test);
}

void main_tests()
{
	int nb = 8;
	// clear_screen(&screen_context); // Commenting this 
	set_color(&screen_context, BG(BLACK) | WHITE);
	// print_f_test();
	// print_k_test();
	// kfs_write_char(&screen_context, '\n');
	print_42();

	// kfs_write_char(&screen_context, 'a');
	// print_f("\n%d-%d\n", screen_context.desktops[0].cursor.x, screen_context.desktops[0].cursor.y);
}
