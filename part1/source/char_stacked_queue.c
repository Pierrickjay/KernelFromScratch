#include "char_stacked_queue.h"
#include "print_manager.h"

t_char_stacked_queue *queue_init(t_char_stacked_queue *queue)
{
	queue->front = queue->rear = queue->data;

	return queue;
}

void queue_push(t_char_stacked_queue *queue, unsigned char data)
{
	*queue->rear = data;
	if (queue->rear == queue->data + QUEUE_SIZE - 1) {
		queue->rear = queue->data;
	}
	else {
		queue->rear++;
	}
}

char queue_pop(t_char_stacked_queue *queue)
{
	char data = *queue->front;
	if (queue->front == queue->data + QUEUE_SIZE - 1) {
		queue->front = queue->data;
	}
	else {
		queue->front++;
	}
	return data;
}

int queue_is_full(t_char_stacked_queue *queue)
{
	unsigned char *next_rear = queue->rear + 1;
	if (next_rear == queue->data + QUEUE_SIZE - 1) {
		next_rear = queue->data;
	}
	return next_rear == queue->front;
}

int queue_is_empty(t_char_stacked_queue *queue)
{
	return queue->front == queue->rear;
}

// @TODO (si tu lis ça dans la PR, il ne faut pas l'accepter mdr) :
// - En spammant à un moment ça décale
// - Gérer les inputs spéciaux comme alt, ctrl etc.. en plus de shift donc penser à un truc global.
//     Même pour les répétitions aussi c'est bien
