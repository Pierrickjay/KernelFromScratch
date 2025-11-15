#include "signal_handler.h"

// Registre global : 256 entrees (une par interruption possible)
interrupt_handler_entry_t interrupt_handlers[MAX_INTERRUPTS];

void init_signal_handler(void)
{
	// Initialise toutes les entrees a NULL/desactive
	for (u16 i = 0; i < MAX_INTERRUPTS; i++) {
		interrupt_handlers[i].callback = NULL;
		interrupt_handlers[i].enabled  = 0;
	}
}

void register_interrupt_handler(u8 num, interrupt_callback_t callback)
{
	// Validation (simple - pas de message d'erreur pour eviter dependances)
	if (callback == NULL) {
		return;
	}

	// Enregistrement
	interrupt_handlers[num].callback = callback;
	interrupt_handlers[num].enabled  = 1;
}

void unregister_interrupt_handler(u8 num)
{
	interrupt_handlers[num].callback = NULL;
	interrupt_handlers[num].enabled  = 0;
}

void dispatch_interrupt_handler(u8 num)
{
	// Verifie si un handler est enregistre ET active
	if (interrupt_handlers[num].enabled && interrupt_handlers[num].callback != NULL) {
		// Appelle le callback
		interrupt_handlers[num].callback();
	}
	// Sinon : interruption non geree, on l'ignore silencieusement
}
