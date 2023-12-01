#pragma once
#include <stdint.h>
#include "../logger/logger.h"
#include "../multiboot.h"
#include "../string/string.h"

extern uintptr_t kernel_end;

void memory_init(multiboot_info_t* mbd);
void* malloc(size_t size);
void free(void* ptr);
void* realloc(void* ptr, size_t size);