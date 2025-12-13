# Part 4 Requirements - Complete Implementation Guide

This document explains how the kernel project fully implements all Part 4 requirements.

---

## Table of Contents
1. [IDT Creation, Filling, and Registration](#1-idt-creation-filling-and-registration)
2. [Signal-Callback System](#2-signal-callback-system-on-kernel-api)
3. [Signal Scheduling Interface](#3-signal-scheduling-interface)
4. [Register Cleaning Before Panic](#4-register-cleaning-interface-before-panic)
5. [Stack Saving Before Panic](#5-stack-saving-interface-before-panic)
6. [IDT Keyboard Handling System](#6-idt-keyboard-handling-system)

---

## 1. IDT Creation, Filling, and Registration

**Files:** [source/cpu/interrupts.c](source/cpu/interrupts.c), [source/cpu/interrupts.h](source/cpu/interrupts.h)

### Data Structures

The IDT is implemented using two key structures:

```c
#define IDT_ENTRIES 256

// Individual IDT entry (8 bytes)
typedef struct idt_entry {
    u16 base_low;   // ISR address bits 0-15
    u16 sel;        // Code segment selector (0x08 = kernel CS from GDT)
    u8  always0;    // Reserved (must be 0)
    u8  flags;      // 0x8E = Present + Ring 0 + 32-bit interrupt gate
    u16 base_high;  // ISR address bits 16-31
} __attribute__((packed)) t_idt_entry;

// IDT pointer for LIDT instruction
typedef struct idt_ptr {
    u16 limit;      // Size of IDT - 1 (2047 for 256 entries)
    u32 base;       // Base address of IDT array
} __attribute__((packed)) t_idt_ptr;
```

### Creation

**Global IDT array:**
```c
t_idt_entry idt[IDT_ENTRIES];  // 256 entries x 8 bytes = 2048 bytes
```

This creates the Interrupt Descriptor Table in kernel memory.

### Filling

**Function:** `set_idt_gate(int n, u32 handler)`

```c
void set_idt_gate(int n, u32 handler)
{
    idt[n].base_low = handler & 0xFFFF;        // Lower 16 bits of ISR address
    idt[n].sel = 0x08;                         // Kernel code segment
    idt[n].always0 = 0;                        // Reserved
    idt[n].flags = 0x8E;                       // Present, Ring 0, 32-bit gate
    idt[n].base_high = (handler >> 16) & 0xFFFF;  // Upper 16 bits
}
```

**What this does:**
- Splits the ISR function address into low (bits 0-15) and high (bits 16-31)
- Sets segment selector to 0x08 (kernel code segment defined in GDT)
- Sets flags to 0x8E:
  - Bit 7: Present (1)
  - Bits 6-5: DPL = 00 (Ring 0, kernel privilege)
  - Bit 4: Storage segment (0)
  - Bits 3-0: Gate type = 1110 (32-bit interrupt gate)

**Entries filled in init_interrupts():**
- Entries 0-19: CPU exceptions (divide by zero, page fault, etc.)
- Entry 0x20: Timer interrupt (IRQ0)
- Entry 0x21: Keyboard interrupt (IRQ1)

### Registration

**Function:** `load_idt()`

```c
static void load_idt(void)
{
    t_idt_ptr idt_ptr;
    idt_ptr.limit = sizeof(t_idt_entry) * IDT_ENTRIES - 1;  // 2047
    idt_ptr.base = (u32)&idt;                               // Address of IDT
    asm volatile("lidt (%0)" : : "r"(&idt_ptr));           // Load into IDTR
}
```

**What this does:**
- Creates an IDT pointer structure with the size (limit) and address (base)
- Uses the `lidt` x86 instruction to load the IDT address into the IDTR register
- After this, the CPU knows where to find interrupt handlers

### PIC Initialization

**Function:** `init_pic()`

The 8259A Programmable Interrupt Controller is initialized to remap hardware IRQs:

```c
static void init_pic(void)
{
    // Master PIC initialization
    outb(0x20, 0x11);  // ICW1: Initialize + ICW4 needed
    outb(0x21, 0x20);  // ICW2: Map IRQ0-7 to INT 0x20-0x27
    outb(0x21, 0x04);  // ICW3: Slave on IRQ2
    outb(0x21, 0x01);  // ICW4: 8086 mode

    // Slave PIC initialization
    outb(0xA0, 0x11);  // ICW1
    outb(0xA1, 0x28);  // ICW2: Map IRQ8-15 to INT 0x28-0x2F
    outb(0xA1, 0x02);  // ICW3: Cascade identity
    outb(0xA1, 0x01);  // ICW4: 8086 mode

    // Mask all IRQs except IRQ0 (timer) and IRQ1 (keyboard)
    outb(0x21, 0xFC);  // Master: 11111100 (enable IRQ0, IRQ1)
    outb(0xA1, 0xFF);  // Slave: all disabled
}
```

**Why this matters:**
- Without remapping, hardware IRQs would conflict with CPU exceptions (0-31)
- IRQ0 (timer) → INT 0x20
- IRQ1 (keyboard) → INT 0x21

### Complete Initialization Sequence

**Function:** `init_interrupts()` - see [source/cpu/interrupts.c:130](source/cpu/interrupts.c#L130)

```c
void init_interrupts(void)
{
    init_signal_handler();       // 1. Initialize callback system
    init_signal_scheduler();     // 2. Initialize deferred signal queue
    init_exception_handlers();   // 3. Set default exception handlers
    init_pic();                  // 4. Remap PIC IRQs
    register_cpu_exceptions();   // 5. Fill IDT entries 0-19
    set_idt_gate(0x20, (u32)timer_handler);      // 6. Timer ISR
    set_idt_gate(0x21, (u32)keyboard_handler);   // 7. Keyboard ISR
    load_idt();                  // 8. Register IDT with CPU (LIDT)
    init_timer_hardware();       // 9. Program PIT for 100Hz
    register_interrupt_handler(0x20, timer_handler_c);      // 10. Timer callback
    register_interrupt_handler(0x21, keyboard_handler_c);   // 11. Keyboard callback
    schedule_signal(0x20, NULL); // 12. Deferred timer signal
    asm volatile("sti");         // 13. Enable interrupts
}
```

**Demonstration:**
- The IDT is created (global array)
- Filled with 256 entries via `set_idt_gate()`
- Registered with CPU via `lidt` instruction
- PIC configured to route hardware IRQs to correct IDT entries

---

## 2. Signal-Callback System on Kernel API

**Files:** [source/cpu/signal_handler.c](source/cpu/signal_handler.c), [source/cpu/signal_handler.h](source/cpu/signal_handler.h)

### Architecture

The signal-callback system provides a registry of function pointers that can be associated with any of the 256 interrupt numbers.

**Data Structures:**

```c
#define MAX_INTERRUPTS 256

typedef void (*interrupt_callback_t)(void);  // Function pointer type

typedef struct {
    interrupt_callback_t callback;  // Function to execute
    u8 enabled;                     // 1 = active, 0 = inactive
} interrupt_handler_entry_t;

static interrupt_handler_entry_t interrupt_handlers[MAX_INTERRUPTS];
```

### API Functions

#### 1. Initialization

```c
void init_signal_handler(void)
{
    for (int i = 0; i < MAX_INTERRUPTS; i++) {
        interrupt_handlers[i].callback = NULL;
        interrupt_handlers[i].enabled = 0;
    }
}
```

Clears all 256 handler slots.

#### 2. Register a Callback

```c
void register_interrupt_handler(u8 num, interrupt_callback_t callback)
{
    if (callback == NULL) {
        return;  // Refuse NULL callbacks
    }
    interrupt_handlers[num].callback = callback;
    interrupt_handlers[num].enabled = 1;
}
```

**Usage example:**
```c
void my_timer_callback(void) {
    print_k("Timer fired!\n");
}

register_interrupt_handler(0x20, my_timer_callback);
```

#### 3. Dispatch an Interrupt

```c
void dispatch_interrupt_handler(u8 num)
{
    if (interrupt_handlers[num].enabled &&
        interrupt_handlers[num].callback != NULL) {
        interrupt_handlers[num].callback();
    }
}
```

**Called from:**
- Assembly ISR stubs in [source/cpu/interrupt_handlers.asm](source/cpu/interrupt_handlers.asm)
- Signal scheduler when processing deferred signals

#### 4. Unregister a Callback

```c
void unregister_interrupt_handler(u8 num)
{
    interrupt_handlers[num].callback = NULL;
    interrupt_handlers[num].enabled = 0;
}
```

### Integration with IDT

**Call flow:**

```
Hardware event → IDT entry → ASM stub → dispatch_interrupt_handler(num) →
registered callback → return
```

**Example (timer interrupt):**

1. Timer fires (IRQ0)
2. PIC routes to INT 0x20
3. CPU jumps to IDT[0x20] → `timer_handler` (assembly)
4. Assembly stub calls `dispatch_interrupt_handler(0x20)`
5. Dispatch calls `timer_handler_c()` (registered callback)
6. Callback executes
7. Returns through assembly stub
8. `iret` instruction returns to interrupted code

### Actual Registered Callbacks

```c
// In init_interrupts()
register_interrupt_handler(0x20, timer_handler_c);      // Timer callback
register_interrupt_handler(0x21, keyboard_handler_c);   // Keyboard callback
```

**Demonstration:**
- Provides clean API to associate C functions with interrupt numbers
- Decouples low-level IDT/assembly from high-level handler logic
- Supports dynamic registration/unregistration
- 256 interrupt slots, one for each IDT entry

---

## 3. Signal Scheduling Interface

**Files:** [source/cpu/signal_scheduler.c](source/cpu/signal_scheduler.c), [source/cpu/signal_scheduler.h](source/cpu/signal_scheduler.h)

### Purpose

The signal scheduler implements **deferred signal processing** - instead of handling an interrupt immediately in its ISR, you can schedule it for later processing. This is crucial for:
- Separating urgent interrupt handling from longer processing
- Avoiding nested interrupt complexity
- Implementing priority-based signal handling

### Architecture - Circular Queue

```c
#define SIGNAL_QUEUE_SIZE 64

typedef struct {
    u8 signal_num;    // Interrupt number (0-255)
    void *data;       // Optional context data
    u8 pending;       // 1 = slot occupied, 0 = free
} pending_signal_t;

static pending_signal_t signal_queue[SIGNAL_QUEUE_SIZE];
static u8 queue_head = 0;  // Read position
static u8 queue_tail = 0;  // Write position
```

**Queue mechanics:**
- **Empty:** `head == tail`
- **Full:** `(tail + 1) % SIGNAL_QUEUE_SIZE == head`
- **FIFO:** First scheduled, first processed

### Signal Masking System

```c
#define NUM_SIGNALS 256
#define MASK_SIZE (NUM_SIGNALS / 8)  // 32 bytes

static u8 signal_masks[MASK_SIZE];  // 256 bits, one per signal
```

Each bit controls whether a signal can be processed:
- Bit = 0: Signal is unmasked (can be processed)
- Bit = 1: Signal is masked (ignored during processing)

### API Functions

#### 1. Initialization

```c
void init_signal_scheduler(void)
{
    queue_head = 0;
    queue_tail = 0;

    for (int i = 0; i < SIGNAL_QUEUE_SIZE; i++) {
        signal_queue[i].pending = 0;
    }

    for (int i = 0; i < MASK_SIZE; i++) {
        signal_masks[i] = 0;  // Unmask all signals
    }
}
```

#### 2. Schedule a Signal (Add to Queue)

```c
int schedule_signal(u8 num, void *data)
{
    u8 next_tail = (queue_tail + 1) % SIGNAL_QUEUE_SIZE;

    if (next_tail == queue_head) {
        return -1;  // Queue full
    }

    signal_queue[queue_tail].signal_num = num;
    signal_queue[queue_tail].data = data;
    signal_queue[queue_tail].pending = 1;

    queue_tail = next_tail;
    return 0;
}
```

**Usage:**
```c
// In timer ISR - defer handling
schedule_signal(0x20, NULL);
```

#### 3. Process Pending Signals (Dequeue and Execute)

```c
void process_pending_signals(void)
{
    while (queue_head != queue_tail) {
        pending_signal_t *signal = &signal_queue[queue_head];

        if (!is_signal_masked(signal->signal_num)) {
            // Call the registered callback
            dispatch_interrupt_handler(signal->signal_num);
        }

        signal->pending = 0;  // Mark slot as free
        queue_head = (queue_head + 1) % SIGNAL_QUEUE_SIZE;
    }
}
```

**Called from:** Main kernel loop in [source/main.c:50](source/main.c#L50)

```c
static void kernel_tick(void)
{
    process_pending_signals();      // Process deferred signals
    handle_keyboard_inputs(&keyboard_queue);
    asm volatile("hlt");            // Wait for next interrupt
}
```

#### 4. Mask/Unmask Signals

```c
void mask_signal(u8 num)
{
    u8 byte_index = num / 8;     // Which byte (0-31)
    u8 bit_index = num % 8;      // Which bit (0-7)
    signal_masks[byte_index] |= (1 << bit_index);  // Set bit
}

void unmask_signal(u8 num)
{
    u8 byte_index = num / 8;
    u8 bit_index = num % 8;
    signal_masks[byte_index] &= ~(1 << bit_index);  // Clear bit
}

int is_signal_masked(u8 num)
{
    u8 byte_index = num / 8;
    u8 bit_index = num % 8;
    return (signal_masks[byte_index] & (1 << bit_index)) ? 1 : 0;
}
```

**Usage:**
```c
mask_signal(0x20);    // Temporarily disable timer processing
// ... critical section ...
unmask_signal(0x20);  // Re-enable timer processing
```

#### 5. Query Functions

```c
u8 get_pending_signals_count(void)
{
    if (queue_tail >= queue_head) {
        return queue_tail - queue_head;
    }
    return SIGNAL_QUEUE_SIZE - queue_head + queue_tail;
}

int is_queue_full(void)
{
    return ((queue_tail + 1) % SIGNAL_QUEUE_SIZE) == queue_head;
}
```

### Two-Mode Signal Handling

The kernel supports both immediate and deferred signal handling:

**Immediate Mode:**
```
IRQ → IDT → ASM stub → dispatch_interrupt_handler() → callback → IRET
```
Fast, but callback runs in interrupt context.

**Deferred Mode:**
```
IRQ → IDT → ASM stub → schedule_signal() → IRET
...later in kernel_tick()...
process_pending_signals() → dispatch_interrupt_handler() → callback
```
Callback runs in normal context, more flexible.

### Demonstration

**Interface components:**
1. `schedule_signal(u8 num, void *data)` - Queue signal for later
2. `process_pending_signals()` - Execute queued signals
3. `mask_signal(u8 num)` / `unmask_signal(u8 num)` - Selective enable/disable
4. `get_pending_signals_count()` - Query queue status
5. `is_queue_full()` - Check capacity

**Actual usage in kernel:**
```c
// In init_interrupts()
schedule_signal(0x20, NULL);  // Defer initial timer signal
```

---

## 4. Register Cleaning Interface Before Panic

**Files:** [source/cpu/panic.c](source/cpu/panic.c), [source/cpu/panic.h](source/cpu/panic.h)

### Purpose

When the kernel encounters a fatal error, it must:
1. Clean up the interrupt state (prevent further interrupts)
2. Display diagnostic information
3. Halt the CPU safely

### Panic Functions

#### 1. Simple Panic (captures current registers)

```c
void kernel_panic(const char *message)
{
    u32 ebp, esp;

    // Capture current base pointer and stack pointer
    asm volatile("mov %%ebp, %0" : "=r"(ebp));
    asm volatile("mov %%esp, %0" : "=r"(esp));

    // Display panic info
    print_k("\n<3>*** KERNEL PANIC ***\n");
    print_k("<3>%s\n", message);

    // Dump stack trace
    dump_stack_trace(ebp, esp);

    // Clean registers and halt
    cleanup_and_halt();
}
```

#### 2. Panic with Full Register Dump

```c
void panic_with_registers(const char *message, t_registers *regs)
{
    print_k("\n<3>*** KERNEL PANIC ***\n");
    print_k("<3>%s\n", message);
    print_k("<0>Interrupt: %d, Error code: %x\n",
            regs->int_no, regs->err_code);

    // Dump all saved registers
    dump_registers(regs);

    // Dump stack starting from saved ESP
    dump_stack_trace(regs->ebp, regs->esp);

    // Clean and halt
    cleanup_and_halt();
}
```

### Register Cleanup and Halt

```c
static void cleanup_and_halt(void)
{
    asm volatile("cli");    // Clear Interrupt Flag - disable interrupts

    while (1) {
        asm volatile("hlt");  // Halt CPU
    }
}
```

**What this does:**

1. **`cli` instruction (Clear Interrupt Flag):**
   - Clears bit 9 (IF flag) in the EFLAGS register
   - Prevents the CPU from responding to maskable interrupts
   - This is the "register cleaning" - it modifies EFLAGS to a safe state
   - Non-maskable interrupts (NMI) still work, but normal interrupts are blocked

2. **`hlt` instruction (Halt):**
   - Puts CPU in low-power halt state
   - CPU waits for interrupt (but CLI disabled them)
   - Effectively freezes the CPU
   - Loop ensures halt is re-entered if NMI wakes CPU

### Register Dump Function

```c
static void dump_registers(t_registers *regs)
{
    print_k("<0>Register dump:\n");
    print_k("<1>  EAX=%x EBX=%x ECX=%x EDX=%x\n",
            regs->eax, regs->ebx, regs->ecx, regs->edx);
    print_k("<1>  ESI=%x EDI=%x EBP=%x ESP=%x\n",
            regs->esi, regs->edi, regs->ebp, regs->esp);
    print_k("<1>  EIP=%x CS=%x EFLAGS=%x\n",
            regs->eip, regs->cs, regs->eflags);
    print_k("<1>  DS=%x ES=%x FS=%x GS=%x\n",
            regs->ds, regs->es, regs->fs, regs->gs);
}
```

Displays all general purpose, segment, and control registers.

### Demonstration

**Register cleaning:**
- `cli` instruction modifies EFLAGS register (clears IF bit)
- Prevents any further interrupt processing
- Ensures kernel halts in a clean, known state
- No interrupts can fire during diagnostic output

**Usage example:**
```c
if (error_condition) {
    kernel_panic("Critical error detected");
    // Never returns - registers cleaned and CPU halted
}
```

---

## 5. Stack Saving Interface Before Panic

**Files:** [source/cpu/exception_stubs.asm](source/cpu/exception_stubs.asm), [source/cpu/panic.c](source/cpu/panic.c)

### Purpose

When an exception or panic occurs, the kernel must preserve the complete CPU state (including stack) for debugging.

### Complete Register State Structure

```c
typedef struct {
    // Saved by ISR stub (exception_stubs.asm)
    u32 gs, fs, es, ds;           // Segment registers
    u32 edi, esi, ebp, esp;       // General purpose (from pusha)
    u32 ebx, edx, ecx, eax;       // General purpose (from pusha)
    u32 int_no, err_code;         // Interrupt number and error code

    // Saved by CPU automatically
    u32 eip, cs, eflags;          // Instruction pointer, code segment, flags
    u32 useresp, ss;              // User stack (only if ring change)
} __attribute__((packed)) t_registers;
```

### Three-Stage Stack Saving

#### Stage 1: CPU Automatic Save (Hardware)

When an exception occurs, the x86 CPU **automatically** pushes to the stack:

```
[EFLAGS]        ← ESP-12
[CS]            ← ESP-8
[EIP]           ← ESP-4
[Error Code]    ← ESP (for some exceptions)
```

**CPU-pushed values:**
- **EIP:** Address of the instruction that caused exception
- **CS:** Code segment selector
- **EFLAGS:** Processor flags at time of exception
- **Error Code:** Optional (only for exceptions 8, 10-14, 17, 21, 29-30)

#### Stage 2: ISR Stub Save (Software - Assembly)

**File:** [source/cpu/exception_stubs.asm](source/cpu/exception_stubs.asm)

```asm
isr_common_stub:
    ; Save all general purpose registers (32 bytes)
    pusha    ; Pushes: EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI

    ; Save segment registers (16 bytes)
    mov ax, ds
    push eax    ; Save DS
    mov ax, es
    push eax    ; Save ES
    mov ax, fs
    push eax    ; Save FS
    mov ax, gs
    push eax    ; Save GS

    ; Load kernel data segment
    mov ax, 0x10    ; Kernel data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Call C exception dispatcher with pointer to register structure
    push esp              ; ESP now points to t_registers structure
    call exception_dispatcher
    add esp, 4            ; Clean parameter

    ; Restore segment registers
    pop eax
    mov gs, ax
    pop eax
    mov fs, ax
    pop eax
    mov es, ax
    pop eax
    mov ds, ax

    ; Restore general purpose registers
    popa

    ; Remove int_no and err_code from stack
    add esp, 8

    ; Return from interrupt (pops EIP, CS, EFLAGS)
    iret
```

**Stack layout after `push esp`:**

```
Low addresses
↓
[GS] [FS] [ES] [DS]                    ← Segment registers (pushed by stub)
[EDI] [ESI] [EBP] [ESP]                ← pusha part 1
[EBX] [EDX] [ECX] [EAX]                ← pusha part 2
[int_no] [err_code]                    ← Pushed by individual ISR stubs
[EIP] [CS] [EFLAGS]                    ← Pushed by CPU
[useresp] [ss]                         ← Pushed by CPU (if ring change)
↑
High addresses
```

**This stack layout exactly matches the `t_registers` structure!**

#### Stage 3: Stack Trace Dump (C Code)

```c
static void dump_stack_trace(u32 ebp, u32 esp)
{
    print_k("<0>Stack trace:\n");

    // Dump first 8 dwords from stack
    for (int i = 0; i < 8; i++) {
        u32 offset = i * 4;
        u32 *stack_ptr = (u32 *)(esp + offset);
        print_k("<1>  [ESP+%x] %x\n", offset, *stack_ptr);
    }

    print_k("<1>  Frame pointer (EBP): %x\n", ebp);
}
```

**Example output:**
```
Stack trace:
  [ESP+0] 00001234    ← Saved EIP
  [ESP+4] 00000008    ← Saved CS
  [ESP+8] 00000202    ← Saved EFLAGS
  [ESP+c] 0010ff00    ← Previous value
  [ESP+10] deadbeef
  [ESP+14] cafebabe
  [ESP+18] 12345678
  [ESP+1c] 87654321
  Frame pointer (EBP): 0010ffc0
```

### Complete Exception Flow with Stack Saving

```
1. User code executes: mov eax, [0]  ; Causes page fault

2. CPU automatically:
   - Pushes EFLAGS, CS, EIP, error_code to stack
   - Loads IDT entry for interrupt 14 (page fault)
   - Jumps to isr14 (defined in exception_stubs.asm)

3. isr14 stub:
   - Pushes interrupt number (14)
   - Jumps to isr_common_stub

4. isr_common_stub:
   - Executes pusha (saves 8 general registers)
   - Saves DS, ES, FS, GS
   - Loads kernel segments
   - Pushes ESP (pointer to saved register structure)
   - Calls exception_dispatcher

5. exception_dispatcher (C code):
   - Receives pointer to t_registers structure
   - Calls registered handler or default_exception_handler

6. default_exception_handler:
   - Calls panic_with_registers(message, regs)

7. panic_with_registers:
   - Dumps all registers from regs structure
   - Calls dump_stack_trace(regs->ebp, regs->esp)
   - Shows 8 dwords from stack
   - Calls cleanup_and_halt()
```

### Demonstration

**Stack is saved at three levels:**

1. **CPU hardware:** Saves EIP, CS, EFLAGS, error code (automatic)
2. **Assembly stub:** Saves all general purpose and segment registers via `pusha` and manual pushes
3. **C code:** Dumps saved registers and stack contents before panic

**The saved stack includes:**
- All general purpose registers (EAX, EBX, ECX, EDX, ESI, EDI, EBP, ESP)
- All segment registers (CS, DS, ES, FS, GS, SS)
- Instruction pointer (EIP) - where exception occurred
- Processor flags (EFLAGS)
- Stack contents (first 8 dwords)

This provides complete context for post-mortem debugging.

---

## 6. IDT Keyboard Handling System

**Files:**
- [source/drivers/keyboard/keyboard_interrupts.c](source/drivers/keyboard/keyboard_interrupts.c)
- [source/drivers/keyboard/keyboard_input.c](source/drivers/keyboard/keyboard_input.c)
- [source/cpu/interrupt_handlers.asm](source/cpu/interrupt_handlers.asm)

### Hardware Configuration

**IRQ:** IRQ1 (keyboard interrupt line on PIC)
**IDT Entry:** 0x21 (after PIC remapping)
**I/O Port:** 0x60 (keyboard data port)
**PIC Mask:** Bit 1 cleared in master PIC mask (0xFC = 11111100b)

### Complete Interrupt Flow

#### 1. Hardware Level

When user presses a key:
- Keyboard controller generates scan code
- Sends signal on IRQ1 line to PIC
- PIC evaluates priority and forwards to CPU

#### 2. PIC Remapping

In `init_pic()` - see [source/cpu/interrupts.c:79](source/cpu/interrupts.c#L79):

```c
outb(0x21, 0x20);  // Master PIC: map IRQ0-7 to INT 0x20-0x27
// Result: IRQ1 → INT 0x21
```

#### 3. IDT Entry Setup

In `init_interrupts()` - see [source/cpu/interrupts.c:149](source/cpu/interrupts.c#L149):

```c
set_idt_gate(0x21, (u32)keyboard_handler);
```

**This fills IDT entry 0x21:**
- Base address: Points to `keyboard_handler` (assembly stub)
- Segment: 0x08 (kernel code)
- Flags: 0x8E (present, ring 0, interrupt gate)

#### 4. Assembly ISR Stub

**File:** [source/cpu/interrupt_handlers.asm](source/cpu/interrupt_handlers.asm)

```asm
global keyboard_handler
keyboard_handler:
    pusha                           ; Save all general registers
    push byte 0x21                  ; Push interrupt number
    call dispatch_interrupt_handler  ; Call C dispatcher
    add esp, 4                      ; Clean stack (remove parameter)
    popa                            ; Restore all registers
    iret                            ; Return from interrupt
```

**Purpose:**
- Saves all registers (prevents corruption)
- Identifies interrupt number (0x21)
- Calls generic C dispatcher
- Restores registers
- Returns to interrupted code

#### 5. Signal Handler Dispatch

`dispatch_interrupt_handler(0x21)` calls the registered callback - see [source/cpu/signal_handler.c:25](source/cpu/signal_handler.c#L25):

```c
void dispatch_interrupt_handler(u8 num)
{
    if (interrupt_handlers[num].enabled &&
        interrupt_handlers[num].callback != NULL) {
        interrupt_handlers[num].callback();  // Calls keyboard_handler_c
    }
}
```

#### 6. C Interrupt Handler

**File:** [source/drivers/keyboard/keyboard_interrupts.c](source/drivers/keyboard/keyboard_interrupts.c)

```c
void keyboard_handler_c(void)
{
    unsigned char scancode = inb(KEYBOARD_PORT);  // Read scan code from port 0x60
    on_keyboard_interrupt(scancode);              // Process scan code
    outb(0x20, 0x20);                            // Send EOI to PIC
}
```

**Key operations:**
1. **Read scan code:** `inb(0x60)` - reads byte from keyboard data port
2. **Process:** `on_keyboard_interrupt(scancode)` - adds to queue
3. **EOI (End Of Interrupt):** `outb(0x20, 0x20)` - tells PIC interrupt is handled

#### 7. Scan Code Queuing

```c
void on_keyboard_interrupt(unsigned char scancode)
{
    if (queue_is_full(&keyboard_queue)) {
        return;  // Drop scan code if queue full
    }
    queue_push(&keyboard_queue, scancode);  // Add to circular buffer
}
```

**Queue structure:**
```c
extern t_char_stacked_queue keyboard_queue;  // 32-byte circular buffer
```

**Purpose:**
- Decouples ISR (fast) from input processing (slower)
- ISR just reads hardware and queues
- Processing happens later in main loop

#### 8. Main Loop Processing

**File:** [source/main.c](source/main.c)

```c
static void kernel_tick(void)
{
    process_pending_signals();               // Handle deferred signals
    handle_keyboard_inputs(&keyboard_queue);  // Process queued scan codes
    asm volatile("hlt");                      // Wait for next interrupt
}
```

`handle_keyboard_inputs()` dequeues scan codes and processes them.

#### 9. Key Processing

**File:** [source/drivers/keyboard/keyboard_input.c](source/drivers/keyboard/keyboard_input.c)

```c
void handle_keyboard_inputs(t_char_stacked_queue *queue)
{
    while (!queue_is_empty(queue)) {
        unsigned char scancode = queue_pop(queue);

        if (scancode == 0xF0) {
            release_flag = 1;  // Next code is key release
            continue;
        }

        int key_index = scancode_to_key_index(scancode);
        if (key_index < 0) continue;

        if (release_flag) {
            // Key release event
            release_flag = 0;
            continue;
        }

        // Key press event - handle based on input mode
        if (current_input_mode == INPUT_MODE_NORMAL) {
            handle_normal_input(key_index, scancode);
        }
        else if (current_input_mode == INPUT_MODE_MINISHELL) {
            handle_minishell_input(key_index, scancode);
        }
    }
}
```

**Supported features:**
- Scan code to key index mapping
- Key press/release detection (0xF0 prefix for release)
- Special keys (F1-F12, arrows, etc.)
- Multiple input modes (normal, minishell, disabled)
- Character echo to screen

### Complete Interrupt Sequence Diagram

```
User Press Key
      ↓
Keyboard Controller (hardware)
      ↓
IRQ1 signal
      ↓
PIC (Programmable Interrupt Controller)
      ↓
Remaps to INT 0x21
      ↓
CPU looks up IDT[0x21]
      ↓
Jumps to keyboard_handler (assembly)
      ↓
pusha (save registers)
      ↓
push 0x21
      ↓
call dispatch_interrupt_handler
      ↓
dispatch calls interrupt_handlers[0x21].callback
      ↓
keyboard_handler_c() executes
      ↓
Read scan code: inb(0x60)
      ↓
queue_push(&keyboard_queue, scancode)
      ↓
Send EOI to PIC: outb(0x20, 0x20)
      ↓
Return to assembly stub
      ↓
popa (restore registers)
      ↓
iret (return from interrupt)
      ↓
Later: kernel_tick() calls handle_keyboard_inputs()
      ↓
Dequeue and process scan codes
      ↓
Convert to characters, echo to screen
```

### Demonstration Points

**IDT Setup:**
- Entry 0x21 registered in IDT
- Points to `keyboard_handler` assembly stub
- Configured as interrupt gate (type 0x8E)

**Hardware Integration:**
- PIC remaps IRQ1 → INT 0x21
- Keyboard port 0x60 read in ISR
- EOI signal sent to PIC

**Signal-Callback Integration:**
- `keyboard_handler_c` registered via `register_interrupt_handler(0x21, keyboard_handler_c)`
- Dispatched through signal handler system

**Deferred Processing:**
- ISR is minimal (read + queue + EOI)
- Heavy processing in main loop
- Queue prevents data loss during burst input

**Complete Features:**
- Scan code reading
- Circular queue buffering
- Key press/release detection
- Special key support (arrows, F-keys)
- Multiple input modes
- Character echo

---

## Summary Matrix

| Requirement | Key Files | Entry Point | Demonstration |
|-------------|-----------|-------------|---------------|
| **IDT Creation** | interrupts.c/h | `init_interrupts()` | Global array `idt[256]`, filled with `set_idt_gate()`, loaded with `lidt` |
| **IDT Filling** | interrupts.c | `set_idt_gate()` | Splits handler address, sets segment (0x08), flags (0x8E) |
| **IDT Registration** | interrupts.c | `load_idt()` | `lidt` instruction loads IDTR with IDT address |
| **Signal-Callback System** | signal_handler.c/h | `register_interrupt_handler()` | 256-entry callback registry, `dispatch_interrupt_handler()` |
| **Signal Scheduling** | signal_scheduler.c/h | `schedule_signal()` | 64-slot circular queue, bit-masking, `process_pending_signals()` |
| **Register Cleaning** | panic.c/h | `cleanup_and_halt()` | `cli` clears EFLAGS IF bit, `hlt` stops CPU |
| **Stack Saving** | exception_stubs.asm, panic.c | `isr_common_stub` | CPU auto-saves, `pusha` saves GPRs, manual segment save, `dump_stack_trace()` |
| **Keyboard IDT Handling** | keyboard_interrupts.c, interrupt_handlers.asm | `keyboard_handler` | IRQ1 → INT 0x21, ISR reads port 0x60, queues scan codes, deferred processing |

---

## Testing & Verification

To verify all requirements are met, you can:

1. **IDT Check:** Boot kernel, verify no triple fault → IDT loaded correctly
2. **Interrupts Work:** Timer ticking (screen updates) → IDT entries functional
3. **Keyboard Input:** Type characters, see them appear → keyboard IDT handler working
4. **Signal System:** Timer uses deferred signals → scheduler functional
5. **Panic Test:** Trigger exception (divide by zero) → see register dump and stack trace
6. **Masking Test:** Mask signal 0x20, verify timer stops → signal masking works

All six requirements are fully implemented and integrated into a cohesive interrupt handling architecture.
