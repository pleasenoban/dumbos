#include "logger.h"

static char* log_texts[] = {
    "[INFO]",
    "[ERROR]",
    "[PANIC]"
};

uint8_t log_colors[] = {
    15, // vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK)
    4, // vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK)
    4 // vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK)
};

void k_log(uint16_t log_level, char* msg) {
    if (log_level == LOGLEVEL_PANIC) {
        asm volatile ("cli");
        terminal_setcolor(log_colors[log_level]);
        printf("%s %s\n", log_texts[log_level], msg);
        asm volatile ("hlt");
    }
    uint8_t _color = terminal_getcolor();
    terminal_setcolor(log_colors[log_level]);
    printf("%s %s\n", log_texts[log_level], msg);
    terminal_setcolor(_color);
}