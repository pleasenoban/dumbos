#pragma once
#include <stdint.h>
#include "../stdio/stdio.h"
#include "../tty/tty.h"

#define LOGLEVEL_INFO 0
#define LOGLEVEL_ERROR 1
#define LOGLEVEL_PANIC 2

void k_log(uint16_t log_level, char* msg);