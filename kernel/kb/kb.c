#include "kb.h"

// http://www.rohitab.com/discuss/topic/39438-keyboard-driver/
static keyevent_t kb_buffer[KB_BUFLEN];
static size_t kb_bufpos = 0;

unsigned long kb_status = 0;

static unsigned char kb_map_en_us[2][128] =
    {
        {0, KB_KEY_ESCAPE, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t', 'q',
         'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd',
         'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b',
         'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, KB_KEY_F1, KB_KEY_F2, KB_KEY_F3, KB_KEY_F4, KB_KEY_F5, KB_KEY_F6,
         KB_KEY_F7, KB_KEY_F8, KB_KEY_F9, KB_KEY_F10, 0, 0, KB_KEY_HOME, KB_KEY_UP, KB_KEY_PGUP, '-', KB_KEY_LEFT, '5', KB_KEY_RIGHT, '+', KB_KEY_END, KB_KEY_DOWN,
         KB_KEY_PGDN, KB_KEY_INSERT, KB_KEY_DELETE, 0, 0, 0, KB_KEY_F11, KB_KEY_F12, 0},
        {0, KB_KEY_ESCAPE, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
         0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0,
         KB_KEY_F1, KB_KEY_F2, KB_KEY_F3, KB_KEY_F4, KB_KEY_F5, KB_KEY_F6, KB_KEY_F7, KB_KEY_F8,
         KB_KEY_F9, KB_KEY_F10, 0, 0, KB_KEY_HOME, KB_KEY_UP, KB_KEY_PGUP, '-', KB_KEY_LEFT, '5', KB_KEY_RIGHT, '+', KB_KEY_END,
         KB_KEY_DOWN, KB_KEY_PGDN, KB_KEY_INSERT, KB_KEY_DELETE, 0, 0, 0, KB_KEY_F11, KB_KEY_F12, 0},
};

// push key into queue.
void kb_pushkey(unsigned char ch, bool released)
{
    kb_buffer[kb_bufpos].key = ch;
    kb_buffer[kb_bufpos].released = released;
    if (kb_bufpos < KB_BUFLEN)
    {
        kb_bufpos++;
    }
}

// pop key from queue
keyevent_t kb_popkey(void)
{
    keyevent_t key = kb_buffer[0];
    if (kb_bufpos > 0)
    {
        kb_bufpos--;
    }
    memmove(&kb_buffer[0], &kb_buffer[1], KB_BUFLEN - 1);
    return key;
}

// output data to the data port
int kb_write(unsigned char data)
{
    unsigned long timeout = 500000;
    while (--timeout)
    {
        if (!(inb(KB_PORT_CMD) & 0x02))
        {
            break;
        }
    }
    if (timeout > 0)
    {
        outb(KB_PORT_DATA, data);
        return 1;
    }
    return 0;
}

// read data from the data port or -1 on error
// int kb_read(void)
// {
//     unsigned long timeout = 500000;
//     while (--timeout)
//     {
//         if (inb(KB_PORT_CMD) & 0x01)
//         {
//             break;
//         }
//     }
//     if (timeout)
//     {
//         return inb(KB_PORT_DATA);
//     }
//     return -1;
// }

// set appropriate keyboad LED's
void kb_setleds(unsigned long status)
{
    // todo: check keyboard controller isn't busy
    // while(!((inb(KB_PORT_CMD) & 2) == 0);
    status &= KB_MASK_LEDS;
    kb_write(KB_CMD_SETLEDS);
    kb_write((unsigned char)status);
}

void kb_handler(struct regs *r)
{
    unsigned char scancode;
    int keycode;
    bool released;

    // read data from keyboard
    keycode = -1;
    scancode = inb(KB_PORT_DATA);
    released = false;

    // key up
    if (scancode & KB_MASK_BREAK)
    {
        released = true;
        scancode &= KB_MASK_KEY;
        switch (scancode)
        {
        // remove bits from the status variable depending
        case KB_KEY_LCTRL:
            kb_status &= ~KB_MASK_CTRL;
            break;
        case KB_KEY_LALT:
            kb_status &= ~KB_MASK_ALT;
            break;
        case KB_KEY_LSHIFT:
        case KB_KEY_RSHIFT:
            kb_status &= ~KB_MASK_SHIFT;
            break;
        default:
            // if capslock or shift is on, use shift keymap
            if (kb_status & KB_MASK_SHIFT || kb_status & KB_MASK_CAPSLOCK)
            {
                keycode = kb_map_en_us[1][scancode];
            }
            // else, use normal keymap
            else
            {
                keycode = kb_map_en_us[0][scancode];
            }
            break;
        }
    }
    // key down
    else
    {
        scancode &= KB_MASK_KEY;
        switch (scancode)
        {
        // add bits to the status variable depending
        case KB_KEY_LCTRL:
            kb_status |= KB_MASK_CTRL;
            break;
        case KB_KEY_LALT:
            kb_status |= KB_MASK_ALT;
            break;
        case KB_KEY_LSHIFT:
        case KB_KEY_RSHIFT:
            kb_status |= KB_MASK_SHIFT;
            break;
        // toggle bits from the status variable depending
        case KB_KEY_SCRLLOCK:
            kb_status ^= KB_MASK_SCRLLOCK;
            kb_setleds(kb_status);
            break;
        case KB_KEY_NUMLOCK:
            kb_status ^= KB_MASK_NUMLOCK;
            kb_setleds(kb_status);
            break;
        case KB_KEY_CAPSLOCK:
            kb_status ^= KB_MASK_CAPSLOCK;
            kb_setleds(kb_status);
            break;
        // for all other keys
        default:
            // if capslock or shift is on, use shift keymap
            if (kb_status & KB_MASK_SHIFT || kb_status & KB_MASK_CAPSLOCK)
            {
                keycode = kb_map_en_us[1][scancode];
            }
            // else, use normal keymap
            else
            {
                keycode = kb_map_en_us[0][scancode];
            }
            break;
        }
    }
    if (keycode != -1)
    {
        // if ctrl-alt-del, then reboot
        if ((kb_status & KB_MASK_CTRL) && (kb_status & KB_MASK_ALT) && (keycode == KB_KEY_DELETE))
        {
            reboot();
        }
        // else push key into queue
        else
        {
            kb_pushkey(keycode, released);
        }
    }
}

void keyboard_install(void)
{
    // clear the queue
    memset(kb_buffer, 0, sizeof(kb_buffer));
    // install the keyboard interupt handler
    irq_install_handler(1, kb_handler);
}

keyevent_t trygetch(void)
{
    // disable ints to avoid race condition
    asm volatile("cli");
    // pop a key from our queue
    keyevent_t key = kb_popkey();
    // enable ints again
    asm volatile("sti");

    return key;
}