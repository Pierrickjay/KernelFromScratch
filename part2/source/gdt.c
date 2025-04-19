#include "gdt.h"
#include "print_manager.h"
/*
 To understand the GDT, you need to understand the following:
	- The GDT is a table of 8-byte entries, that the processor uses to determine what memory
 segments are accessible.
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

// GDT Entries
t_gdt_entry gdt_start[7];
// GDT Descriptor
t_gdt_ptr gdt_descriptor = {
	.limit = sizeof(gdt_start) - 1,	 // Size of GDT
	.base  = (unsigned int)gdt_start // Address of first GDT entry
};

void gdt_set_value(int num, unsigned int base, unsigned int limit, unsigned char access,
				   unsigned char gran)
{
	gdt_start[num].base_low	   = (base & 0xFFFF);
	gdt_start[num].base_middle = (base >> 16) & 0xFF;
	gdt_start[num].base_high   = (base >> 24) & 0xFF;
	gdt_start[num].limit_low   = (limit & 0xFFFF);
	gdt_start[num].granularity = (limit >> 16) & 0x0F;
	gdt_start[num].granularity |= gran & 0xF0;
	gdt_start[num].access = access;
}

/*Called my main*/
void gdt_install()
{
	// KERNEL PART

	gdt_set_value(0, 0, 0, 0, 0); // Null segment

	/* The second entry is our Code Segment. The base address
	 *  is 0, the limit is 4GBytes, it uses 4KByte granularity,
	 *  uses 32-bit opcodes, and is a Code Segment descriptor. */
	gdt_set_value(1, 0, 0xFFFFFFFF, (unsigned char)GDT_CODE_PL0, 0xCF); // Code Kernel segment

	/* The third entry is our Data Segment. It's EXACTLY the
	 *  same as our code segment, but the descriptor type in
	 *  this entry's access byte says it's a Data Segment */
	gdt_set_value(2, 0, 0xFFFFFFFF, (unsigned char)GDT_DATA_PL0, 0xCF); // Data Kernel segment

	gdt_set_value(3, 0, 0xFFFFFFFF, (unsigned char)GDT_STACK_PL0, 0xCF); // Stack Kernel segment

	// USER PART

	/* The second entry is our Code Segment. The base address
	 *  is 0, the limit is 4GBytes, it uses 4KByte granularity,
	 *  uses 32-bit opcodes, and is a Code Segment descriptor. */
	gdt_set_value(4, 0, 0xFFFFFFFF, (unsigned char)GDT_CODE_PL3, 0xCF); // Code User segment

	/* The third entry is our Data Segment. It's EXACTLY the
	 *  same as our code segment, but the descriptor type in
	 *  this entry's access byte says it's a Data Segment */
	gdt_set_value(5, 0, 0xFFFFFFFF, (unsigned char)GDT_DATA_PL3, 0xCF);
	; // Data segment

	gdt_set_value(6, 0, 0xFFFFFFFF, (unsigned char)GDT_STACK_PL3, 0xCF);
}

void print_gdt_summary() // ca va pas ici
{
	print_k(KERN_INFO "GDT Summary:\n");
	for (int i = 0; i < sizeof(gdt_start) / sizeof(t_gdt_entry); i++)
	{
		print_k(KERN_INFO "Entry %d: Base=0x%x%x%x Limit=0x%x%x Access=0x%x\n", i,
			   gdt_start[i].base_high, gdt_start[i].base_middle, gdt_start[i].base_low,
			   gdt_start[i].granularity & 0x0F, gdt_start[i].limit_low, gdt_start[i].access);
	}
}
