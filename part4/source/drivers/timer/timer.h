#ifndef TIMER_H
#define TIMER_H

#include "types.h"

// Frequence du PIT (Programmable Interval Timer)
#define PIT_FREQUENCY 1193182 // Hz
#define TIMER_FREQ	  100	  // Interruptions par seconde (10ms par tick)

// Ports du PIT
#define PIT_CHANNEL0 0x40 // Channel 0 data port
#define PIT_COMMAND	 0x43 // Mode/Command register

// Handler en C pour le timer
void timer_handler_c(void);

// Initialise le timer
void init_timer(void);

// Recupere le nombre de ticks depuis le demarrage
u32 get_timer_ticks(void);

#endif
