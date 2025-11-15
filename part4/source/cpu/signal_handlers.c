#include "print_manager.h"
#include "signal_scheduler.h"
#include "signals.h"

// Compteur pour debug
static int timer_signal_count = 0;

void handle_timer_signal(void)
{
	timer_signal_count++;
	print_k(KERN_NOTICE "\n[Differe] Timer signal periodique ! Count: %d\n",
			timer_signal_count);
	print_k(KERN_INFO "Signal 0x%x traite depuis process_pending_signals()\n",
			SIGNAL_TIMER);
	print_k(KERN_DEBUG "Signaux restants en queue: %d\n",
			get_pending_signals_count());
}
