#pragma once

// much information was taken from
// http://www.osdever.net/bkerndev/Docs/gdt.htm

/* Explanation of padding : https://stackoverflow.com/questions/5473189/what-is-a-packed-structure-in-c
We pad in because we need to prevent compiler to do some optimization
*/

struct gdt_entry
{
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
}__attribute__((packed));

struct gdt_ptr
{
	unsigned short limit;
	unsigned int bas;
}__attribute__((packed));

void gdt_install();

extern void _gdt_flush();

#define null 0,0,0,0
