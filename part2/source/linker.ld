
OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
 {
   . = 1M;
   .text BLOCK(4K) : ALIGN(4K)
   {
       *(.multiboot)
       *(.text)
   }
   .data : { *(.data) }
   .bss  : { *(.bss)  }

   . = 0x00000800;
   .gdt : {
       gdt_start = .;
       . += 56; /* Number of GDT entries * size of each entry */
   }
 }
