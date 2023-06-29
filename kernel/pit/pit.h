#pragma once
#include "../isr/isr.h"
#include "../irq/irq.h"
#include "../io/io.h"
#include "../stdio/stdio.h"
#include "../string/string.h"

void timer_install();
void timer_wait(int ticks);