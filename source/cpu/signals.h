#ifndef SIGNALS_H
#define SIGNALS_H

// Definition des signaux custom (au-dessus des interruptions materielles)
// On utilise la plage 0xA0-0xFF pour les signaux software

#define SIGNAL_TIMER  0xA0 // Timer tick periodique (differe)
#define SIGNAL_CUSTOM 0xA1 // Signal custom pour tests

#endif
