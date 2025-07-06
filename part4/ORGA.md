To do :

Summary Table
| Requirement | Implementation Steps/Files |
|----------------------------|-------------------------------------------|
| IDT | interrupts.h, interrupts.c, ASM |
| Signal-callback system | New signals.h/signals.c |
| Schedule signals | Add queue/flag, process in main loop |
| Clean registers | New ASM function, call in panic handler |
| Save stack | New C function, call in panic handler |