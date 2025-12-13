#ifndef CPU_EXCEPTION_HANDLERS_H
#define CPU_EXCEPTION_HANDLERS_H

#include "exception_handlers.h"

void page_fault_handler(t_registers *regs);
void general_protection_fault_handler(t_registers *regs);
void segment_not_present_handler(t_registers *regs);
void division_by_zero_handler(t_registers *regs);
void invalid_opcode_handler(t_registers *regs);

void register_cpu_exception_handlers(void);

#endif
