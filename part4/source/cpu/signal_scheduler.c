#include "signal_scheduler.h"
#include "signal_handler.h"

// Queue circulaire pour les signaux pendants
static pending_signal_t signal_queue[SIGNAL_QUEUE_SIZE];
static u8				queue_head = 0; // Index de lecture
static u8				queue_tail = 0; // Index d'ecriture

// Tableau de masques : 256 bits = 32 bytes (1 bit par interruption)
static u8 signal_masks[32];

void init_signal_scheduler(void)
{
	// Initialise la queue a vide
	for (u16 i = 0; i < SIGNAL_QUEUE_SIZE; i++) {
		signal_queue[i].signal_num = 0;
		signal_queue[i].data	   = NULL;
		signal_queue[i].pending	   = 0;
	}

	queue_head = 0;
	queue_tail = 0;

	// Initialise tous les masques a 0 (non masques)
	for (u8 i = 0; i < 32; i++) {
		signal_masks[i] = 0;
	}
}

int schedule_signal(u8 num, void *data)
{
	// Verifie si la queue est pleine
	u8 next_tail = (queue_tail + 1) % SIGNAL_QUEUE_SIZE;
	if (next_tail == queue_head) {
		// Queue pleine : ignore le nouveau signal (strategie recommandee)
		return -1;
	}

	// Ajoute le signal a la queue
	signal_queue[queue_tail].signal_num = num;
	signal_queue[queue_tail].data		= data;
	signal_queue[queue_tail].pending	= 1;

	// Avance le tail
	queue_tail = next_tail;

	return 0;
}

void process_pending_signals(void)
{
	// Traite tous les signaux en attente
	while (queue_head != queue_tail) {
		// Recupere le signal
		pending_signal_t *sig = &signal_queue[queue_head];

		// Verifie si le signal est masque
		if (!is_signal_masked(sig->signal_num)) {
			// Appelle le dispatcher (qui appellera le callback)
			dispatch_interrupt_handler(sig->signal_num);
		}

		// Marque le slot comme libre
		sig->pending = 0;

		// Avance le head
		queue_head = (queue_head + 1) % SIGNAL_QUEUE_SIZE;
	}
}

void mask_signal(u8 num)
{
	// Calcule l'index du byte et du bit
	u8 byte_index = num / 8;
	u8 bit_index  = num % 8;

	// Met le bit a 1 (masque)
	signal_masks[byte_index] |= (1 << bit_index);
}

void unmask_signal(u8 num)
{
	// Calcule l'index du byte et du bit
	u8 byte_index = num / 8;
	u8 bit_index  = num % 8;

	// Met le bit a 0 (non masque)
	signal_masks[byte_index] &= ~(1 << bit_index);
}

int is_signal_masked(u8 num)
{
	// Calcule l'index du byte et du bit
	u8 byte_index = num / 8;
	u8 bit_index  = num % 8;

	// Retourne 1 si masque, 0 sinon
	return (signal_masks[byte_index] & (1 << bit_index)) ? 1 : 0;
}

u16 get_pending_signals_count(void)
{
	// Compte le nombre de signaux en attente
	if (queue_tail >= queue_head) {
		return queue_tail - queue_head;
	} else {
		// Queue circulaire wrappee
		return (SIGNAL_QUEUE_SIZE - queue_head) + queue_tail;
	}
}

int is_queue_full(void)
{
	u8 next_tail = (queue_tail + 1) % SIGNAL_QUEUE_SIZE;
	return (next_tail == queue_head) ? 1 : 0;
}
