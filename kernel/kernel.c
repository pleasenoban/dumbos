#include "gdt/gdt.h"
#include "idt/idt.h"
#include "isr/isr.h"
#include "irq/irq.h"
#include "pit/pit.h"
#include "kb/kb.h"
#include "mouse/mouse.h"
#include "tty/tty.h"
#include "stdio/stdio.h"
#include "string/string.h"
 
void kernel_main(void) 
{
	// gdt & idt & isr & irq
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	asm volatile ("sti");
	// pit
	timer_install();
	// kb
	keyboard_install();
	// mouse
	mouse_install();
	/* Initialize terminal interface */
	terminal_initialize();

	// terminal_disable_cursor();
	printf("hewo world :3\n");
}