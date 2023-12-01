#include "mm.h"

// i think i need to rework this sometime in the future
static uintptr_t memory_end_address = 0;
static uintptr_t g_heap = 0;

#define ALIGNMENT 8

void memory_init(multiboot_info_t *mbd)
{
    kernel_end = (uintptr_t)(&kernel_end);
    if (!(mbd->flags >> 6 & 0x1))
        k_log(LOGLEVEL_PANIC, "invalid memory map given");
    for (uint32_t i = 0; i < mbd->mmap_length;
         i += sizeof(multiboot_memory_map_t))
    {
        multiboot_memory_map_t *mmmt =
            (multiboot_memory_map_t *)(mbd->mmap_addr + i);

        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE &&
            (uintptr_t)(mmmt->addr + mmmt->len) > memory_end_address)
        {
            memory_end_address = (uintptr_t)(mmmt->addr + mmmt->len);
        }
    }
    g_heap = kernel_end;
}

// napalm http://www.rohitab.com/discuss/topic/31090-c-os-dev-malloc-and-free/
void *sbrk(uint32_t nbytes)
{
    uintptr_t alloc = g_heap;
    g_heap += nbytes;
    if (g_heap >= memory_end_address)
        return NULL; // No more heap
    return (void *)alloc;
}

// sylvain and kingkong https://stackoverflow.com/questions/5422061/malloc-implementation
typedef struct free_block {
    size_t size;
    struct free_block* next;
} free_block;

static free_block free_block_list_head = { 0, 0 };

void* malloc(size_t size) {
    // align trick thing
    size = (size + sizeof(free_block) + (ALIGNMENT - 1)) & ~ (ALIGNMENT - 1);
    free_block* block = free_block_list_head.next;
    free_block** head = &(free_block_list_head.next);
    while (block != 0) {
        if (block->size >= size) {
            *head = block->next;
            return ((char*)block) + sizeof(free_block);
        }
        head = &(block->next);
        block = block->next;
    }

    block = (free_block*)sbrk(size);
    if (block == NULL)
        k_log(LOGLEVEL_PANIC, "out of memory");
    block->size = size;

    return ((char*)block) + sizeof(free_block);
}

void free(void* ptr) {
    free_block* block = (free_block*)(((char*)ptr) - sizeof(free_block));
    block->next = free_block_list_head.next;
    free_block_list_head.next = block;
}

void* realloc(void* ptr, size_t size) {
    if (ptr == NULL)
        return malloc(size);
    void* dst = malloc(size);
    memcpy(dst, ptr, ((free_block*)(((char*)ptr) - sizeof(free_block)))->size);
    free(ptr);
    return dst;
}