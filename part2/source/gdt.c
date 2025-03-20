#include "gdt.h"

struct gdt_entry gdt[3];
struct gdt_ptr   _gp;

/*
 To understand the GDT, you need to understand the following:
    - The GDT is a table of 8-byte entries, that the processor uses to determine what memory segments are accessible.
    - Each entry in the GDT is 8 bytes long, and has the following structure:
        - 0-15 bits: Segment limit
        - 16-31 bits: Base address
        - 32-39 bits: Base address
        - 40-47 bits: Access flags
        - 48-55 bits: Granularity
        - 56-63 bits: Base address
    - The segment limit is a 20-bit value that specifies the size of the segment in bytes.
    - The base address is a 32-bit value that specifies the starting address of the segment.
    - The access flags are a set of 8 bits that specify the access rights for the segment.
    - The granularity is a set of 8 bits that specify the size of the segment.
    - The base address is a 32-bit value that specifies the starting address of the segment.
*/
void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
    gdt[num].base_low	 = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high	 = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;


    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access = access;
}

/*Called my main*/
void gdt_install()
{
    _gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    _gp.bas = (unsigned int)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0); // Null segment
    /* The second entry is our Code Segment. The base address
    *  is 0, the limit is 4GBytes, it uses 4KByte granularity,
    *  uses 32-bit opcodes, and is a Code Segment descriptor.
    *  Please check the table above in the tutorial in order
    *  to see exactly what each value means */
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    /* The third entry is our Data Segment. It's EXACTLY the
    *  same as our code segment, but the descriptor type in
    *  this entry's access byte says it's a Data Segment */
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment

    /*Flush the old gdt (created by grub) and install the new one*/
    _gdt_flush();
}
