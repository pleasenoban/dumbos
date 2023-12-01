#include <stdint.h>
#include "multiboot.h"
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
#include "logger/logger.h"
#include "memory/mm.h"
#include "graphics/graphics.h"

void kernel_init(multiboot_info_t* mbd)
{
	// gdt & idt & isr & irq
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	asm volatile("sti");
	/* Initialize terminal interface */
	terminal_initialize();
	k_log(LOGLEVEL_INFO, "terminal initialized");
	// pit
	timer_install();
	k_log(LOGLEVEL_INFO, "pit timer initialized");
	// kb
	keyboard_install();
	k_log(LOGLEVEL_INFO, "keyboard initialized");
	// mouse
	mouse_install();
	k_log(LOGLEVEL_INFO, "mouse initialized");
	mouse_set_sens(0.05);
	k_log(LOGLEVEL_INFO, "mouse sens set");
	// memory
	memory_init(mbd);
	k_log(LOGLEVEL_INFO, "memory manager initialized");
	k_log(LOGLEVEL_INFO, "kernel initialized successfully");
}

void kernel_main(multiboot_info_t* mbd, uint32_t magic)
{
	// check if kernel is booted by a multiboot-compliant bootloader
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
		k_log(LOGLEVEL_PANIC, "magic number doesnt match");
	kernel_init(mbd);
	int32_test();
	k_log(LOGLEVEL_INFO, "int32 test done");
	keyevent_t test;
	while (true) {
		test = trygetch();
		if (test.key != 0) {
			printf("key %c %s %s\n", test.key, itoa(test.released, 2), ultoa(kb_status, 2));
		}
	}
	// terminal_disable_cursor();
}