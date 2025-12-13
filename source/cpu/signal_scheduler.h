#ifndef SIGNAL_SCHEDULER_H
#define SIGNAL_SCHEDULER_H

#include "types.h"
#include <stddef.h>

#define SIGNAL_QUEUE_SIZE 64

// Signal en attente dans la queue
typedef struct {
	u8	  signal_num; // Numero d'interruption (0-255)
	void *data;		  // Donnees optionnelles (peut être NULL)
	u8	  pending;	  // 1 = en attente, 0 = slot libre
} pending_signal_t;

// Initialise le systeme de signal scheduler
void init_signal_scheduler(void);

// Ajoute un signal a la queue (mode differe)
// Retourne 0 si succes, -1 si queue pleine
int schedule_signal(u8 num, void *data);

// Traite tous les signaux en attente dans la queue
void process_pending_signals(void);

// Masquage de signaux
void mask_signal(u8 num);	 // Ignore temporairement ce signal
void unmask_signal(u8 num);	 // Reactive le traitement
int	 is_signal_masked(u8 num); // Verifie si masque (1=oui, 0=non)

// Statistiques (optionnel, pour debug)
u16 get_pending_signals_count(void);
int is_queue_full(void);

#endif