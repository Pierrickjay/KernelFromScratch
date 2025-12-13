#ifndef EXCEPTION_HANDLERS_H
#define EXCEPTION_HANDLERS_H

#include "types.h"

typedef enum {
	CPU_EXCEPTION_DIVIDE_BY_ZERO			  = 0x00,
	CPU_EXCEPTION_DEBUG						  = 0x01,
	CPU_EXCEPTION_NMI						  = 0x02,
	CPU_EXCEPTION_BREAKPOINT				  = 0x03,
	CPU_EXCEPTION_OVERFLOW					  = 0x04,
	CPU_EXCEPTION_BOUND_RANGE_EXCEEDED		  = 0x05,
	CPU_EXCEPTION_INVALID_OPCODE			  = 0x06,
	CPU_EXCEPTION_DEVICE_NOT_AVAILABLE		  = 0x07,
	CPU_EXCEPTION_DOUBLE_FAULT				  = 0x08,
	CPU_EXCEPTION_COPROCESSOR_SEGMENT_OVERRUN = 0x09,
	CPU_EXCEPTION_INVALID_TSS				  = 0x0A,
	CPU_EXCEPTION_SEGMENT_NOT_PRESENT		  = 0x0B,
	CPU_EXCEPTION_STACK_SEGMENT_FAULT		  = 0x0C,
	CPU_EXCEPTION_GENERAL_PROTECTION_FAULT	  = 0x0D,
	CPU_EXCEPTION_PAGE_FAULT				  = 0x0E,
	CPU_EXCEPTION_RESERVED					  = 0x0F,
	CPU_EXCEPTION_X87_FPU_ERROR				  = 0x10,
	CPU_EXCEPTION_ALIGNMENT_CHECK			  = 0x11,
	CPU_EXCEPTION_MACHINE_CHECK				  = 0x12,
	CPU_EXCEPTION_SIMD_FLOATING_POINT		  = 0x13
} t_cpu_exception;

typedef struct {
	u32 gs, fs, es, ds;
	u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
	u32 int_no, err_code;
	u32 eip, cs, eflags, useresp, ss;
} __attribute__((packed)) t_registers;

typedef void (*t_exception_handler)(t_registers *regs);

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);

void exception_dispatcher(t_registers *regs);
void register_exception_handler(u8 exception_num, t_exception_handler handler);
void init_exception_handlers(void);
const char *get_exception_name(u8 exception_num);

#endif
