#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

#define QUEUE_SIZE 32

typedef struct char_stacked_queue {
	unsigned char  data[QUEUE_SIZE];
	unsigned char *front;
	unsigned char *rear;
} t_char_stacked_queue;

t_char_stacked_queue *queue_init(t_char_stacked_queue *queue);
void				  queue_push(t_char_stacked_queue *queue, unsigned char data);
char				  queue_pop(t_char_stacked_queue *queue);
int					  queue_is_full(t_char_stacked_queue *queue);
int					  queue_is_empty(t_char_stacked_queue *queue);

#endif
