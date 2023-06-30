#include "mouse.h"

uint8_t mouse_cycle = 0;
int8_t mouse_byte[3];
int8_t delta_mouse_x = 0;
int8_t delta_mouse_y = 0;
int32_t mouse_x = 0;
int32_t mouse_y = 0;
double mouse_sens = 1;

// Mouse functions
void mouse_handler(struct regs *r) // struct regs *r (not used but just there)
{
    switch (mouse_cycle)
    {
    case 0:
        mouse_byte[0] = inb(0x60);
        mouse_cycle++;
        break;
    case 1:
        mouse_byte[1] = inb(0x60);
        mouse_cycle++;
        break;
    case 2:
        mouse_byte[2] = inb(0x60);
        delta_mouse_x = mouse_byte[1];
        delta_mouse_y = mouse_byte[2];
        mouse_x += (int32_t)((int32_t)delta_mouse_x * mouse_sens);
        mouse_y -= (int32_t)((int32_t)delta_mouse_y * mouse_sens);
        mouse_x = clamp(mouse_x, 0, 79);
        mouse_y = clamp(mouse_y, 0, 24);
        // printf("x: %s, y: ", itoa(mouse_x, 10));
        // printf("%s\ndelta x: ", itoa(mouse_y, 10));
        // printf("%s, delta y: ", itoa(delta_mouse_x, 10));
        // printf("%s\n", itoa(delta_mouse_y, 10));
        printf("\n");
        terminal_putentryat((char)219, VGA_COLOR_BLUE, mouse_x, mouse_y);
        mouse_cycle = 0;
        break;
    }
}

static inline void mouse_wait(uint8_t a_type)
{
    uint32_t _time_out = 100000;
    if (a_type == 0)
    {
        while (_time_out--) // Data
        {
            if ((inb(0x64) & 1) == 1)
            {
                return;
            }
        }
        return;
    }
    else
    {
        while (_time_out--) // Signal
        {
            if ((inb(0x64) & 2) == 0)
            {
                return;
            }
        }
        return;
    }
}

static inline void mouse_write(uint8_t a_write)
{
    // Wait to be able to send a command
    mouse_wait(1);
    // Tell the mouse we are sending a command
    outb(0x64, 0xD4);
    // Wait for the final part
    mouse_wait(1);
    // Finally write
    outb(0x60, a_write);
}

static inline uint8_t mouse_read()
{
    // Get's response from mouse
    mouse_wait(0);
    return inb(0x60);
}

void mouse_install()
{
    uint8_t _status;

    // Enable the auxiliary mouse device
    mouse_wait(1);
    outb(0x64, 0xA8);

    // Enable the interrupts
    mouse_wait(1);
    outb(0x64, 0x20);
    mouse_wait(0);
    _status = (inb(0x60) | 2);
    mouse_wait(1);
    outb(0x64, 0x60);
    mouse_wait(1);
    outb(0x60, _status);

    // Tell the mouse to use default settings
    mouse_write(0xF6);
    mouse_read(); // Acknowledge

    // Enable the mouse
    mouse_write(0xF4);
    mouse_read(); // Acknowledge

    // Setup the mouse handler
    irq_install_handler(12, mouse_handler);
}

void mouse_set_sens(double sens)
{
    mouse_sens = sens;
}