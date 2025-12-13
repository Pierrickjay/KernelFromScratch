# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

KernelFromScratch is a 42 school project implementing an x86 32-bit kernel from scratch. The repository contains progressive parts (part1, part2, part4) with increasing functionality. Development happens primarily in **part4/** which is the most advanced.

## Build Commands

```bash
# Build ISO (from partX/ directory)
make

# Run in QEMU with KVM
make virtualisation

# Run with serial output to file
make log

# Format code
make format

# Clean
make clean    # Remove object files
make fclean   # Remove object files and ISO
```

## Architecture

### Boot Flow
```
GRUB → boot.asm → gdt_install() → main()
                                   ├─ init_screen_context()
                                   ├─ init_interrupts()
                                   ├─ queue_init()
                                   └─ kernel_tick() loop (handle inputs + hlt)
```

### Memory Layout (linker.ld)
- Kernel loaded at **1MB** (0x100000)
- GDT placed at **0x800**
- Output format: ELF32 i386

### Source Structure (part4/source/)
- **boot/** - Multiboot ASM, linker script, GRUB config
- **cpu/** - GDT, IDT, interrupts, exception handlers (ISR 0-19), panic system
- **display/** - VGA text mode (80x25), cursor, print functions, 4 virtual desktops
- **drivers/** - Keyboard (QWERTY layout, input queue) and serial port
- **io/** - Low-level port I/O (inb/outb)
- **lib/** - Utilities (memset, strlen, strcmp, circular queue)
- **shell/** - Mini-shell
- **tests/** - Kernel test suite including exception tests

### Key Types (types.h)
Custom integer types: `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`

### Compilation
- **C**: GCC with `-fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs -m32`
- **ASM**: NASM with `-f elf32`
- **Linker**: LD with `-m elf_i386`

## C Coding Conventions

- Use `static` for internal utility functions (don't expose in .h files)
- No inline comments - prefer extracting well-named functions
- Comments explain "why", never "what"
- Code must be self-documenting through naming and structure
- Return meaningful values (0/1) for success/failure or operation nature
