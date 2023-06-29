#pragma once
#include "../isr/isr.h"
#include "../idt/idt.h"
#include "../io/io.h"

void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
void irq_install();