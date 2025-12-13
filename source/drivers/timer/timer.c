#include "timer.h"
#include "io.h"
#include "print_manager.h"
#include "signal_scheduler.h"
#include "signals.h"

// Compteur de ticks depuis le demarrage
static volatile u32 timer_ticks = 0;

// Frequence pour generer un signal differe (toutes les N interruptions)
#define SIGNAL_EVERY_N_TICKS 1300 // Toutes les secondes (400 ticks = 1s a 400Hz)

void init_timer(void)
{
	// Calcule le diviseur pour obtenir la frequence souhaitee
	u16 divisor = PIT_FREQUENCY / TIMER_FREQ;

	// Configure le PIT en mode 3 (square wave generator)
	// Command byte: 0x36 = 00 11 011 0
	//   00 = Channel 0
	//   11 = Access mode: lobyte/hibyte
	//   011 = Mode 3 (square wave)
	//   0 = Binary mode
	outb(PIT_COMMAND, 0x36);

	// Envoie le diviseur (low byte puis high byte)
	outb(PIT_CHANNEL0, (u8)(divisor & 0xFF));
	outb(PIT_CHANNEL0, (u8)((divisor >> 8) & 0xFF));

	timer_ticks = 0;

	print_k(KERN_INFO "[TIMER] Initialise a %d Hz \n",
					TIMER_FREQ, 1000 / TIMER_FREQ);
}

void timer_handler_c(void)
{
	timer_ticks++;

	// Toutes les N interruptions, schedule un signal differe
	if (timer_ticks % SIGNAL_EVERY_N_TICKS == 0)
	{
		int result = schedule_signal(SIGNAL_TIMER, NULL);
		if (result == 0)
		{
			print_k(KERN_DEBUG "[ISR] Timer tick %d -> signal 0x%x ajoute a la queue\n",
							timer_ticks, SIGNAL_TIMER);
		}
	}

	// Signale au PIC que l'interruption est traitee
	outb(0x20, 0x20);
}

u32 get_timer_ticks(void)
{
	return timer_ticks;
}
