#ifndef IO_H
#define IO_H

#include "types.h" // Si tu utilises des types custom (u8, u16, etc.)

// Écrire un octet (byte) sur le port spécifié
void outb(u16 port, u8 data);

// Lire un octet (byte) depuis le port spécifié
u8 inb(u16 port);

#endif
