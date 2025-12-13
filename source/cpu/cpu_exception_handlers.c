#include "cpu_exception_handlers.h"
#include "exception_handlers.h"
#include "panic.h"
#include "print_manager.h"

void page_fault_handler(t_registers *regs)
{
	u32 faulting_address;
	asm volatile("mov %%cr2, %0" : "=r"(faulting_address));

	int present  = !(regs->err_code & 0x1);
	int write    = regs->err_code & 0x2;
	int user     = regs->err_code & 0x4;
	int reserved = regs->err_code & 0x8;
	int fetch    = regs->err_code & 0x10;

	print_k("<0>Exception %x: %s at address 0x%x\n", regs->int_no,
			get_exception_name(regs->int_no), faulting_address);
	print_k("<1>  Cause: %s\n", present ? "Page not present" : "Protection violation");
	print_k("<1>  Access: %s\n", write ? "Write" : "Read");
	print_k("<1>  Mode: %s\n", user ? "User" : "Kernel");
	if (reserved) {
		print_k("<1>  Reserved bits overwritten\n");
	}
	if (fetch) {
		print_k("<1>  Instruction fetch\n");
	}

	panic_with_registers("Page Fault", regs);
}

void general_protection_fault_handler(t_registers *regs)
{
	int external       = regs->err_code & 0x1;
	int table_type     = (regs->err_code >> 1) & 0x3;
	int selector_index = (regs->err_code >> 3);

	const char *table_names[] = {"GDT", "IDT", "LDT", "IDT"};

	print_k("<0>Exception %x: %s\n", regs->int_no, get_exception_name(regs->int_no));
	print_k("<1>  Error Code: 0x%x\n", regs->err_code);
	print_k("<1>  Table: %s\n", table_names[table_type]);
	print_k("<1>  Selector Index: %d (0x%x)\n", selector_index, selector_index);
	print_k("<1>  External: %s\n", external ? "Yes" : "No");

	panic_with_registers("General Protection Fault", regs);
}

void segment_not_present_handler(t_registers *regs)
{
	int external       = regs->err_code & 0x1;
	int table_type     = (regs->err_code >> 1) & 0x3;
	int selector_index = (regs->err_code >> 3);

	const char *table_names[] = {"GDT", "IDT", "LDT", "IDT"};

	print_k("<0>Exception %x: %s\n", regs->int_no, get_exception_name(regs->int_no));
	print_k("<1>  Error Code: 0x%x\n", regs->err_code);
	print_k("<1>  Table: %s\n", table_names[table_type]);
	print_k("<1>  Selector Index: %d (0x%x)\n", selector_index, selector_index);
	print_k("<1>  External: %s\n", external ? "Yes" : "No");
	print_k("<1>  Reason: Segment descriptor present bit is 0\n");

	panic_with_registers("Segment Not Present", regs);
}

void division_by_zero_handler(t_registers *regs)
{
	print_k("<0>Exception %x: %s\n", regs->int_no, get_exception_name(regs->int_no));
	print_k("<1>  Division by zero detected at EIP: 0x%x\n", regs->eip);

	panic_with_registers("Division By Zero", regs);
}

void invalid_opcode_handler(t_registers *regs)
{
	print_k("<0>Exception %x: %s\n", regs->int_no, get_exception_name(regs->int_no));
	print_k("<1>  Invalid instruction at EIP: 0x%x\n", regs->eip);

	panic_with_registers("Invalid Opcode", regs);
}

void register_cpu_exception_handlers(void)
{
	register_exception_handler(CPU_EXCEPTION_DIVIDE_BY_ZERO, division_by_zero_handler);
	register_exception_handler(CPU_EXCEPTION_INVALID_OPCODE, invalid_opcode_handler);
	register_exception_handler(CPU_EXCEPTION_SEGMENT_NOT_PRESENT, segment_not_present_handler);
	register_exception_handler(CPU_EXCEPTION_GENERAL_PROTECTION_FAULT,
							   general_protection_fault_handler);
	register_exception_handler(CPU_EXCEPTION_PAGE_FAULT, page_fault_handler);
}
