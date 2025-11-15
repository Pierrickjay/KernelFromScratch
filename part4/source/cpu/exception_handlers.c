#include "exception_handlers.h"
#include "panic.h"
#include "print_manager.h"

#define MAX_EXCEPTION_HANDLERS 32

static t_exception_handler exception_handlers[MAX_EXCEPTION_HANDLERS];

static const char *exception_names[] = {"Division By Zero",
										"Debug",
										"Non Maskable Interrupt",
										"Breakpoint",
										"Overflow",
										"Bound Range Exceeded",
										"Invalid Opcode",
										"Device Not Available",
										"Double Fault",
										"Coprocessor Segment Overrun",
										"Invalid TSS",
										"Segment Not Present",
										"Stack Segment Fault",
										"General Protection Fault",
										"Page Fault",
										"Reserved",
										"x87 FPU Error",
										"Alignment Check",
										"Machine Check",
										"SIMD Floating Point Exception"};

static void default_exception_handler(t_registers *regs)
{
	const char *name = "Unknown Exception";

	if (regs->int_no < 20) {
		name = exception_names[regs->int_no];
	}

	if (regs->int_no == CPU_EXCEPTION_PAGE_FAULT) {
		u32 faulting_address;
		asm volatile("mov %%cr2, %0" : "=r"(faulting_address));
		print_k("<0>Exception %x: %s at address %x\n", regs->int_no, name, faulting_address);
	}
	else {
		print_k("<0>Exception %x: %s\n", regs->int_no, name);
	}

	panic_with_registers(name, regs);
}

void exception_dispatcher(t_registers *regs)
{
	if (regs->int_no < MAX_EXCEPTION_HANDLERS && exception_handlers[regs->int_no] != 0) {
		exception_handlers[regs->int_no](regs);
	}
	else {
		default_exception_handler(regs);
	}
}

void register_exception_handler(u8 exception_num, t_exception_handler handler)
{
	if (exception_num < MAX_EXCEPTION_HANDLERS) {
		exception_handlers[exception_num] = handler;
	}
}

void init_exception_handlers(void)
{
	for (int i = 0; i < MAX_EXCEPTION_HANDLERS; i++) {
		exception_handlers[i] = 0;
	}
}
