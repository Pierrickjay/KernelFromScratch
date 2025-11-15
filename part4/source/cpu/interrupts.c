#include "interrupts.h"
#include "exception_handlers.h"
#include "io.h"
#include "keyboard_interrupts.h"
#include "signal_handler.h"
#include "signal_handlers.h"
#include "signal_scheduler.h"
#include "signals.h"
#include "timer.h"
#include "types.h"
#include "utils.h"

t_idt_entry idt[IDT_ENTRIES];

void set_idt_gate(int n, u32 handler)
{
	idt[n].base_low	 = handler & 0xFFFF;
	idt[n].sel		 = 0x08; // Selecteur du segment de code kernel dans GDT
	idt[n].always0	 = 0;
	idt[n].flags	 = 0x8E; // Present, ring 0, type=32bit interrupt gate
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

	// Masques : Activer le timer (IRQ0) et le clavier (IRQ1)
	outb(0x21, 0xFC); // 11111100 : IRQ0 (timer) et IRQ1 (clavier) activees
	outb(0xA1, 0xFF); // Masque tout cote esclave
}

static void register_cpu_exceptions(void)
{
	set_idt_gate(0, (u32)isr0);
	set_idt_gate(1, (u32)isr1);
	set_idt_gate(2, (u32)isr2);
	set_idt_gate(3, (u32)isr3);
	set_idt_gate(4, (u32)isr4);
	set_idt_gate(5, (u32)isr5);
	set_idt_gate(6, (u32)isr6);
	set_idt_gate(7, (u32)isr7);
	set_idt_gate(8, (u32)isr8);
	set_idt_gate(9, (u32)isr9);
	set_idt_gate(10, (u32)isr10);
	set_idt_gate(11, (u32)isr11);
	set_idt_gate(12, (u32)isr12);
	set_idt_gate(13, (u32)isr13);
	set_idt_gate(14, (u32)isr14);
	set_idt_gate(15, (u32)isr15);
	set_idt_gate(16, (u32)isr16);
	set_idt_gate(17, (u32)isr17);
	set_idt_gate(18, (u32)isr18);
	set_idt_gate(19, (u32)isr19);
}

void init_interrupts(void)
{
	init_signal_handler();

	init_signal_scheduler();

	init_exception_handlers();
	init_pic();

	set_idt_gate(0x20, (u32)timer_handler);
	register_cpu_exceptions();
	set_idt_gate(0x21, (u32)keyboard_handler);

	load_idt();

	// Initialise le timer (PIT)
	init_timer();

	// Enregistre les callbacks des interruptions matérielles (mode immediat)
	register_interrupt_handler(0x20, timer_handler_c);
	register_interrupt_handler(0x21, keyboard_handler_c);

	// Enregistre le handler pour le signal timer (mode differe)
	register_interrupt_handler(SIGNAL_TIMER, handle_timer_signal);

	// Active les interruptions
	asm volatile("sti");
}
