kfs1 explanation :
Summary Table
| Requirement | File(s) Involved | What it Does |
|-------------------------------------|---------------------------------|---------------------------------------------------|
| Boot via GRUB | grub.cfg, Makefile | Loads your kernel as a multiboot image |
| ASM bootable base | boot.asm, interrupt_handlers.asm | Sets up CPU, handles interrupts |
| Basic kernel library | types.h, utils.c, print_manager.c | Provides types and utility functions |
| Print to screen | screen_manager.c, print_manager.c | Writes text to VGA buffer |
| "Hello world" kernel | main.c | Initializes everything and prints a message |

linker.ld explanation : 

Summary Table
| Directive | Purpose |
|-------------------|----------------------------------------------|
| ENTRY(start) | Sets the entry point for the kernel |
| . = 1M; | Loads the kernel at 1MB in memory |
| .text | Contains code and multiboot header |
| .rodata | Contains read-only data |
| .data | Contains initialized global/static variables |
| .bss | Contains uninitialized global/static vars |
_
