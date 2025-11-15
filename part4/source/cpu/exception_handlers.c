#include "exception_handlers.h"
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

static void print_register_dump(t_registers *regs)
{
	print_k("<1>EAX=%x EBX=%x ECX=%x EDX=%x\n", regs->eax, regs->ebx, regs->ecx, regs->edx);
	print_k("<1>ESI=%x EDI=%x EBP=%x ESP=%x\n", regs->esi, regs->edi, regs->ebp, regs->esp);
	print_k("<1>EIP=%x CS=%x EFLAGS=%x\n", regs->eip, regs->cs, regs->eflags);
	print_k("<1>DS=%x ES=%x FS=%x GS=%x\n", regs->ds, regs->es, regs->fs, regs->gs);
}

static void default_exception_handler(t_registers *regs)
{
	const char *name = "Unknown Exception";

	if (regs->int_no < 20) {
		name = exception_names[regs->int_no];
	}

	print_k("<0>*** CPU EXCEPTION %x: %s ***\n", regs->int_no, name);

	if (regs->int_no == CPU_EXCEPTION_PAGE_FAULT) {
		u32 faulting_address;
		asm volatile("mov %%cr2, %0" : "=r"(faulting_address));
		print_k("<1>Faulting address: %x\n", faulting_address);
	}

	if (regs->err_code != 0) {
		print_k("<1>Error code: %x\n", regs->err_code);
	}

	print_register_dump(regs);

	print_k("<0>System halted.\n");

	asm volatile("cli");
	asm volatile("hlt");
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
