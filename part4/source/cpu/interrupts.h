#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"

#define IDT_ENTRIES 256

// Interrupt Descriptor Table
typedef struct idt_entry {
	u16 base_low; // ISR address low base (Interrupt Service Routine)
	u16 sel;	  // Code segment (CS)
	u8	always0;
	u8	flags;
	u16 base_high; // ISR address high base
} __attribute__((packed)) t_idt_entry;

typedef struct idt_ptr {
	u16 limit;
	u32 base;
} __attribute__((packed)) t_idt_ptr;

void init_interrupts(void);

// Handlers ASM
extern void timer_handler(void);
extern void keyboard_handler(void);

#endif
