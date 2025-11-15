#ifndef PANIC_H
#define PANIC_H

#include "exception_handlers.h"
#include "types.h"

void kernel_panic(const char *message);
void panic_with_registers(const char *message, t_registers *regs);

#endif
