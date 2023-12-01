#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>

void *memmove(void *dstptr, const void *srcptr, size_t size);
size_t strlen(const char *str);
int memcmp(const void *aptr, const void *bptr, size_t size);
void *memset(void *bufptr, int value, size_t size);
void *memcpy(void *restrict dstptr, const void *restrict srcptr, size_t size);
int atoi(const char *str);
char *itoa(int val, int base);
char *ulltoa(uint64_t val, int base);
char *ultoa(uint32_t val, int base);