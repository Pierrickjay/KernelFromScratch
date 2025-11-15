#ifndef IO_H
#define IO_H

#include "types.h" // Si tu utilises des types custom (u8, u16, etc.)

// ecrire un octet (byte) sur le port specifie
void outb(u16 port, u8 data);

// Lire un octet (byte) depuis le port specifie
u8 inb(u16 port);

#endif
