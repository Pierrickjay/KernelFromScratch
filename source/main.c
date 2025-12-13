#include "interrupts.h"
#include "keyboard_input.h"
#include "keyboard_interrupts.h"
#include "print_manager.h"
#include "screen_manager.h"
#include "serial.h"
#include "signal_scheduler.h"
#include "tests.h"

static void kernel_tick(void)
{
	// Traite tous les signaux en attente (mode differe)
	process_pending_signals();

	// Gere les entrees clavier
	handle_keyboard_inputs(&keyboard_queue);

	// Met le CPU en pause jusqu'a la prochaine interruption
	asm volatile("hlt");
}

void main()
{
	// serial_init(SERIAL_COM1_BASE); // debug only
	init_screen_context(&screen_context);
	init_interrupts();
	queue_init(&keyboard_queue);

	main_tests();

	set_input_mode(INPUT_MODE_NORMAL);

	while (1) {
		kernel_tick();
	}
}
