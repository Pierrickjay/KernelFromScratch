#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include "types.h"
#include <stddef.h>

#define MAX_INTERRUPTS 256

// Type: pointeur vers fonction callback d'interruption
typedef void (*interrupt_callback_t)(void);

// Entree du registre pour une interruption
typedef struct {
	interrupt_callback_t callback; // Fonction a appeler
	u8					 enabled;  // 1 = actif, 0 = desactive
} interrupt_handler_entry_t;

// Initialise le systeme de signal handler
void init_signal_handler(void);

// Enregistre un handler pour une interruption donnee
void register_interrupt_handler(u8 num, interrupt_callback_t callback);

// Desenregistre un handler pour une interruption donnee
void unregister_interrupt_handler(u8 num);

// Dispatch une interruption vers son handler enregistre
void dispatch_interrupt_handler(u8 num);

#endif
