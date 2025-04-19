#include "io.h"

// Fonction d'écriture d'un octet sur un port
void outb(u16 port, u8 data)
{
	asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

// Fonction de lecture d'un octet depuis un port
u8 inb(u16 port)
{
	u8 ret;
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}