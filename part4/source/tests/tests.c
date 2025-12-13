#include "print_manager.h"
#include "signal_handler.h"
#include "signal_scheduler.h"

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
	print_k(KERN_NOTICE "System initialization %s complete %s with %d seconds\n", "test",
			"successfully", 123);
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
	int test		= print_f("ceci est un string test %s\n", "frefjreferf");
	int writed_char = print_f("ceci est un hex test %x\n", &nb);
	print_f("nb of writed char: %d\n", writed_char);
	print_string("Versiwfwfefewfwefewoweewefwefew\n");
	print_number(test);
}

// Variables globales pour les tests de callback
static int test_callback1_called = 0;
static int test_callback2_called = 0;

// Callbacks de test
void test_callback1(void)
{
	test_callback1_called++;
}

void test_callback2(void)
{
	test_callback2_called++;
}

void test_signal_handler_system(void)
{
	print_k(KERN_INFO "\n=== [B1] Signal Handler System Tests ===\n");

	// Test 1: Enregistrement d'un handler
	print_k(KERN_INFO "Test 1: Enregistrement d'un handler... ");
	register_interrupt_handler(0x80, test_callback1);
	print_k(KERN_INFO "[OK]\n");

	// Test 2: Enregistrement de plusieurs handlers
	print_k(KERN_INFO "Test 2: Enregistrement de plusieurs handlers... ");
	register_interrupt_handler(0x81, test_callback2);
	register_interrupt_handler(0x82, test_callback1);
	print_k(KERN_INFO "[OK]\n");

	// Test 3: Appel direct du dispatcher
	print_k(KERN_INFO "Test 3: Test du dispatcher (0x80)... ");
	test_callback1_called = 0;
	dispatch_interrupt_handler(0x80);
	if (test_callback1_called == 1) {
		print_k(KERN_INFO "[OK] - Callback appele\n");
	}
	else {
		print_k(KERN_ERR "[FAIL] - Callback non appele\n");
	}

	// Test 4: Appel d'un autre handler
	print_k(KERN_INFO "Test 4: Test du dispatcher (0x81)... ");
	test_callback2_called = 0;
	dispatch_interrupt_handler(0x81);
	if (test_callback2_called == 1) {
		print_k(KERN_INFO "[OK] - Callback appele\n");
	}
	else {
		print_k(KERN_ERR "[FAIL] - Callback non appele\n");
	}

	// Test 5: Desenregistrement d'un handler
	print_k(KERN_INFO "Test 5: Desenregistrement d'un handler... ");
	unregister_interrupt_handler(0x80);
	test_callback1_called = 0;
	dispatch_interrupt_handler(0x80);
	if (test_callback1_called == 0) {
		print_k(KERN_INFO "[OK] - Callback desactive\n");
	}
	else {
		print_k(KERN_ERR "[FAIL] - Callback encore actif\n");
	}

	// Test 6: Handler non enregistre (doit être ignore)
	print_k(KERN_INFO "Test 6: Interruption non geree (0xFF)... ");
	dispatch_interrupt_handler(0xFF);
	print_k(KERN_INFO "[OK] - Ignoree sans crash\n");

	// Test 7: Re-enregistrement
	print_k(KERN_INFO "Test 7: Re-enregistrement d'un handler... ");
	register_interrupt_handler(0x80, test_callback1);
	test_callback1_called = 0;
	dispatch_interrupt_handler(0x80);
	if (test_callback1_called == 1) {
		print_k(KERN_INFO "[OK] - Callback reactive\n");
	}
	else {
		print_k(KERN_ERR "[FAIL] - Callback non reactive\n");
	}

	// Test 8: Verification du handler clavier (IRQ1 = 0x21)
	print_k(KERN_INFO "Test 8: Verification du handler clavier (0x21)... [OK]\n");
	print_k(KERN_NOTICE "Testez en tapant au clavier maintenant!\n");

	print_k(KERN_INFO "\n=== Tests termines ===\n\n");
}

// Variables pour les tests du scheduler
static int scheduler_callback_count = 0;

void scheduler_test_callback(void)
{
	scheduler_callback_count++;
}

void test_signal_scheduler_system(void)
{
	print_k(KERN_INFO "\n=== [B2] Signal Scheduler System Tests ===\n");

	// Test 1: Enregistrement d'un handler pour mode differe
	print_k(KERN_INFO "Test 1: Enregistrement handler pour scheduler... ");
	register_interrupt_handler(0x90, scheduler_test_callback);
	print_k(KERN_INFO "[OK]\n");

	// Test 2: Schedule un signal
	print_k(KERN_INFO "Test 2: Schedule un signal (0x90)... ");
	scheduler_callback_count = 0;
	int result				 = schedule_signal(0x90, NULL);
	if (result == 0) {
		print_k(KERN_INFO "[OK] - Signal ajoute\n");
	}
	else {
		print_k(KERN_ERR "[FAIL] - Erreur schedule\n");
	}

	// Test 3: Verifie que le callback n'est PAS encore appele (differe)
	print_k(KERN_INFO "Test 3: Callback pas encore appele... ");
	if (scheduler_callback_count == 0) {
		print_k(KERN_INFO "[OK] - Traitement differe\n");
	}
	else {
		print_k(KERN_ERR "[FAIL] - Callback appele trop tôt\n");
	}

	// Test 4: Compte les signaux pendants
	print_k(KERN_INFO "Test 4: Compte signaux pendants... ");
	u16 pending = get_pending_signals_count();
	if (pending == 1) {
		print_k(KERN_INFO "[OK] - 1 signal en attente\n");
	}
	else {
		print_k(KERN_ERR "[FAIL] - %d signaux au lieu de 1\n", pending);
	}

	// Test 5: Traite les signaux pendants
	print_k(KERN_INFO "Test 5: Process pending signals... ");
	scheduler_callback_count = 0;
	process_pending_signals();
	if (scheduler_callback_count == 1) {
		print_k(KERN_INFO "[OK] - Callback appele\n");
	}
	else {
		print_k(KERN_ERR "[FAIL] - Callback non appele\n");
	}

	// Test 6: Queue vide apres traitement
	print_k(KERN_INFO "Test 6: Queue vide apres traitement... ");
	pending = get_pending_signals_count();
	if (pending == 0) {
		print_k(KERN_INFO "[OK] - Queue vide\n");
	}
	else {
		print_k(KERN_ERR "[FAIL] - %d signaux restants\n", pending);
	}

	// Test 7: Schedule multiples signaux
	print_k(KERN_INFO "Test 7: Schedule 5 signaux... ");
	for (int i = 0; i < 5; i++) {
		schedule_signal(0x90, NULL);
	}
	pending = get_pending_signals_count();
	if (pending == 5) {
		print_k(KERN_INFO "[OK] - 5 signaux en queue\n");
	}
	else {
		print_k(KERN_ERR "[FAIL] - %d signaux au lieu de 5\n", pending);
	}

	// Test 8: Traite tous les signaux
	print_k(KERN_INFO "Test 8: Traite tous les signaux... ");
	scheduler_callback_count = 0;
	process_pending_signals();
	if (scheduler_callback_count == 5) {
		print_k(KERN_INFO "[OK] - 5 callbacks appeles\n");
	}
	else {
		print_k(KERN_ERR "[FAIL] - %d callbacks au lieu de 5\n", scheduler_callback_count);
	}

	// Test 9: Masquage de signal
	print_k(KERN_INFO "Test 9: Masquage de signal (0x90)... ");
	mask_signal(0x90);
	if (is_signal_masked(0x90)) {
		print_k(KERN_INFO "[OK] - Signal masque\n");
	}
	else {
		print_k(KERN_ERR "[FAIL] - Signal non masque\n");
	}

	// Test 10: Signal masque non traite
	print_k(KERN_INFO "Test 10: Signal masque ignore... ");
	scheduler_callback_count = 0;
	schedule_signal(0x90, NULL);
	process_pending_signals();
	if (scheduler_callback_count == 0) {
		print_k(KERN_INFO "[OK] - Callback ignore\n");
	}
	else {
		print_k(KERN_ERR "[FAIL] - Callback appele malgre masque\n");
	}

	// Test 11: Demasquage
	print_k(KERN_INFO "Test 11: Demasquage de signal... ");
	unmask_signal(0x90);
	if (!is_signal_masked(0x90)) {
		print_k(KERN_INFO "[OK] - Signal demasque\n");
	}
	else {
		print_k(KERN_ERR "[FAIL] - Signal encore masque\n");
	}

	// Test 12: Signal demasque traite
	print_k(KERN_INFO "Test 12: Signal demasque traite... ");
	scheduler_callback_count =
		0; // Configure l'IDT pour le timer (IRQ0 = 0x20) et le clavier (IRQ1 = 0x21)
	schedule_signal(0x90, NULL);
	process_pending_signals();
	if (scheduler_callback_count == 1) {
		print_k(KERN_INFO "[OK] - Callback appele\n");
	}
	else {
		print_k(KERN_ERR "[FAIL] - Callback non appele\n");
	}

	// Test 13: Overflow de la queue (remplir la queue)
	print_k(KERN_INFO "Test 13: Test overflow queue... ");
	int overflow_result = 0;
	for (int i = 0; i < SIGNAL_QUEUE_SIZE + 5; i++) {
		overflow_result = schedule_signal(0x90, NULL);
	}
	if (overflow_result == -1) {
		print_k(KERN_INFO "[OK] - Overflow detecte\n");
	}
	else {
		print_k(KERN_ERR "[FAIL] - Overflow non detecte\n");
	}

	// Nettoie la queue
	process_pending_signals();

	print_k(KERN_INFO "\n=== Tests Scheduler termines ===\n\n");
}

void test_division_by_zero(void)
{
	print_k("<2>Testing division by zero exception...\n");
	volatile int a = 42;
	volatile int b = 0;
	volatile int c = a / b;
	print_k("<2>This should never print: %d\n", c);
}

void test_invalid_opcode(void)
{
	print_k("<2>Testing Invalid Opcode exception...\n");
	asm volatile("ud2");
	print_k("<2>This should never print\n");
}

void test_breakpoint(void)
{
	print_k("<2>Testing Breakpoint exception...\n");
	asm volatile("int $3");
	print_k("<2>After breakpoint (should see this if handled correctly)\n");
}

void test_segment_not_present(void)
{
	print_k("<2>Testing Segment Not Present (with error code)...\n");
	asm volatile("mov $0xFFFF, %ax\n"
				 "mov %ax, %ds");
	print_k("<2>This should never print\n");
}

void main_tests()
{
	int nb = 8;
	set_color(&screen_context, BG(BLACK) | WHITE);
	// print_42();
	// print_k_test();

	// Test du systeme de signal scheduler [B2]
	test_signal_scheduler_system();

	// test_division_by_zero(); // Will cause a kernel panic
}
