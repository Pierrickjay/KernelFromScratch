#include "panic.h"
#include "print_manager.h"

static void dump_registers(t_registers *regs)
{
	print_k("<0>Registers dump:\n");
	print_k("<1>  EAX=%x  EBX=%x  ECX=%x  EDX=%x\n", regs->eax, regs->ebx, regs->ecx,
			regs->edx);
	print_k("<1>  ESI=%x  EDI=%x  EBP=%x  ESP=%x\n", regs->esi, regs->edi, regs->ebp,
			regs->esp);
	print_k("<1>  EIP=%x  CS=%x   EFLAGS=%x\n", regs->eip, regs->cs, regs->eflags);
	print_k("<1>  DS=%x   ES=%x   FS=%x   GS=%x\n", regs->ds, regs->es, regs->fs, regs->gs);

	if (regs->err_code != 0) {
		print_k("<1>  Error Code: %x\n", regs->err_code);
	}
}

static void dump_stack_trace(u32 ebp, u32 esp)
{
	print_k("<0>Stack trace:\n");

	for (int i = 0; i < 8; i++) {
		u32 offset = i * 4;
		u32 *stack_ptr = (u32 *)(esp + offset);
		print_k("<1>  [ESP+%x] %x\n", offset, *stack_ptr);
	}

	print_k("<1>  Frame pointer (EBP): %x\n", ebp);
}

static void cleanup_and_halt(void)
{
	print_k("<0>\n");
	print_k("<0>System halted. Press reset to reboot.\n");

	asm volatile("cli");

	while (1) {
		asm volatile("hlt");
	}
}

void panic_with_registers(const char *message, t_registers *regs)
{
	print_k("<0>\n");
	print_k("<0>+-------------------------------------------------------+\n");
	print_k("<0>|           *** KERNEL PANIC ***                        |\n");
	print_k("<0>+-------------------------------------------------------+\n");
	print_k("<0>| %s\n", message);
	print_k("<0>+-------------------------------------------------------+\n");
	print_k("<0>\n");

	if (regs != 0) {
		dump_registers(regs);
		print_k("<0>\n");
		dump_stack_trace(regs->ebp, regs->esp);
	}

	cleanup_and_halt();
}

void kernel_panic(const char *message)
{
	u32 ebp, esp;

	asm volatile("mov %%ebp, %0" : "=r"(ebp));
	asm volatile("mov %%esp, %0" : "=r"(esp));

	print_k("<0>\n");
	print_k("<0>+-------------------------------------------------------+\n");
	print_k("<0>|           *** KERNEL PANIC ***                        |\n");
	print_k("<0>+-------------------------------------------------------+\n");
	print_k("<0>| %s\n", message);
	print_k("<0>+-------------------------------------------------------+\n");
	print_k("<0>\n");

	dump_stack_trace(ebp, esp);

	cleanup_and_halt();
}
