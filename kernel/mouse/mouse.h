#pragma once

#include <stdint.h>
#include "../io/io.h"
#include "../irq/irq.h"
#include "../stdio/stdio.h"
#include "../string/string.h"
#include "../math/math.h"

void mouse_install();
void mouse_set_sens();
// void mouse_set_sample_rate(uint8_t sample_rate);