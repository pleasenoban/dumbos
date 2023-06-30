#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <limits.h>
#include "../string/string.h"
#include "../tty/tty.h"

#define EOF (-1)

int putchar(int ic);
int printf(const char *restrict format, ...);
int puts(const char *string);