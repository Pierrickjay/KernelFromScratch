#include "panic.h"
#include "print_manager.h"
#include "serial.h"

#define panic_log(level, fmt, ...)                                                                 \
	do {                                                                                           \
		print_k("<" level ">" fmt, ##__VA_ARGS__);                                                 \
		print_serial(fmt, ##__VA_ARGS__);                                                          \
	} while (0)

static void dump_registers(t_registers *regs)
{
	panic_log("0", "Registers dump:\n");
	panic_log("1", "  EAX=%x  EBX=%x  ECX=%x  EDX=%x\n", regs->eax, regs->ebx, regs->ecx, regs->edx);
	panic_log("1", "  ESI=%x  EDI=%x  EBP=%x  ESP=%x\n", regs->esi, regs->edi, regs->ebp, regs->esp);
	panic_log("1", "  EIP=%x  CS=%x   EFLAGS=%x\n", regs->eip, regs->cs, regs->eflags);
	panic_log("1", "  DS=%x   ES=%x   FS=%x   GS=%x\n", regs->ds, regs->es, regs->fs, regs->gs);

	if (regs->err_code != 0) {
		panic_log("1", "  Error Code: %x\n", regs->err_code);
	}
}

static void dump_stack_trace(u32 ebp, u32 esp)
{
	panic_log("0", "Stack trace:\n");

	for (int i = 0; i < 8; i++) {
		u32	 offset	   = i * 4;
		u32 *stack_ptr = (u32 *)(esp + offset);
		panic_log("1", "  [ESP+%x] %x\n", offset, *stack_ptr);
	}

	panic_log("1", "  Frame pointer (EBP): %x\n", ebp);
}

static void cleanup_and_halt(void)
{
	panic_log("0", "\nSystem halted. Press reset to reboot.\n");

	asm volatile("cli\n"
				 "xor %%eax, %%eax\n"
				 "xor %%ebx, %%ebx\n"
				 "xor %%ecx, %%ecx\n"
				 "xor %%edx, %%edx\n"
				 "xor %%esi, %%esi\n"
				 "xor %%edi, %%edi\n" ::
					 : "eax", "ebx", "ecx", "edx", "esi", "edi");

	while (1) {
		asm volatile("hlt");
	}
}

void panic_with_registers(const char *message, t_registers *regs)
{
	panic_log("0", "\n+-------------------------------------------------------+\n");
	panic_log("0", "|           *** KERNEL PANIC ***                        |\n");
	panic_log("0", "+-------------------------------------------------------+\n");
	panic_log("0", "| %s\n", message);
	panic_log("0", "+-------------------------------------------------------+\n\n");

	if (regs != 0) {
		dump_registers(regs);
		panic_log("0", "\n");
		dump_stack_trace(regs->ebp, regs->esp);
	}

	cleanup_and_halt();
}

void kernel_panic(const char *message)
{
	u32 ebp, esp;

	asm volatile("mov %%ebp, %0" : "=r"(ebp));
	asm volatile("mov %%esp, %0" : "=r"(esp));

	panic_log("0", "\n+-------------------------------------------------------+\n");
	panic_log("0", "|           *** KERNEL PANIC ***                        |\n");
	panic_log("0", "+-------------------------------------------------------+\n");
	panic_log("0", "| %s\n", message);
	panic_log("0", "+-------------------------------------------------------+\n\n");

	dump_stack_trace(ebp, esp);

	cleanup_and_halt();
}
