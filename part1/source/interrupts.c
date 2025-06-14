#include "interrupts.h"
#include "io.h"
#include "keyboard_interrupts.h"
#include "types.h"
#include "utils.h"

t_idt_entry idt[IDT_ENTRIES];

void set_idt_gate(int n, u32 handler)
{
	idt[n].base_low	 = handler & 0xFFFF;
	idt[n].sel		 = 0x08; // Sélecteur du segment de code kernel dans GDT
	idt[n].always0	 = 0;
	idt[n].flags	 = 0x8E; // Présent, ring 0, type=32bit interrupt gate
	idt[n].base_high = (handler >> 16) & 0xFFFF;
}

void load_idt()
{
	t_idt_ptr idtp;

	idtp.limit = (sizeof(t_idt_entry) * IDT_ENTRIES) - 1;
	idtp.base  = (u32)&idt;
	asm volatile("lidt (%0)" : : "r"(&idtp));
}

void init_pic(void)
{
	// Initialisation PIC maître
	outb(0x20, 0x11); // ICW1 : initialisation
	outb(0x21, 0x20); // ICW2 : offset IDT (0x20)
	outb(0x21, 0x04); // ICW3 : indique qu'il y a un PIC esclave (IRQ2)
	outb(0x21, 0x01); // ICW4 : mode 8086

	// Initialisation PIC esclave
	outb(0xA0, 0x11); // ICW1 : initialisation
	outb(0xA1, 0x28); // ICW2 : offset IDT pour l'esclave (0x28)
	outb(0xA1, 0x02); // ICW3 : indique sa position d'esclave (cascade IRQ2)
	outb(0xA1, 0x01); // ICW4 : mode 8086

	// Masques : Activer uniquement le clavier (IRQ1)
	outb(0x21, 0xFD); // 11111101 : IRQ1 (clavier) activée, reste masqué
	outb(0xA1, 0xFF); // Masque tout côté esclave
}

void init_interrupts(void)
{
	init_pic();

	set_idt_gate(0x21, (u32)keyboard_handler);
	load_idt();

	asm volatile("sti");
}
