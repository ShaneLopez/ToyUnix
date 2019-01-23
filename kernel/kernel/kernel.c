#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <kernel/tty.h>
#include <keyboard_map.h>
#include <kernel/descriptor_tables.h>

#define INT_DISABLE 0
#define INT_ENABLE 0x200
#define PIC1 0x20
#define PIC2 0xA0
#define ICW1 0x11
#define ICW4 0x01

extern unsigned char keyboard_map[128];

void outb(unsigned short port, unsigned char val)
{
	asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static __inline unsigned char inb(unsigned short int port)
{
	unsigned char _v;

	asm volatile("inb %w1, %0" : "=a"(_v) : "Nd"(port));

	return _v;
}

void init_pics(int pic1, int pic2)
{
	outb(PIC1, ICW1);
	outb(PIC2, ICW1);

	outb(PIC1 + 1, pic1);
	outb(PIC2 + 1, pic2);

	outb(PIC1 + 1, 4);
	outb(PIC2 + 1, 2);

	outb(PIC1 + 1, ICW4);
	outb(PIC2 + 1, ICW4);

	outb(PIC1 + 1, 0xFF);
}

void kernel_main(void)
{
	char c = 0;
	terminal_initialize();
	init_descriptor_tables();
	init_pics(0x20, 0x28);
	outb(0x21, 0xFD);

	do
	{

		if(inb(0x60) != c)
		{
			c = inb(0x60);
			if(c > 0)
				terminal_putchar(keyboard_map[(unsigned char) c]);
		}

		outb(0x20, 0x20);
	}while(c != 1);
}
