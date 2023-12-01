#include "graphics.h"

void int32_test()
{
    srandt();
    regs16_t regs;
     
    // switch to 320x200x256 graphics mode
    regs.ax = 0x0013;
    int32(0x10, &regs);
     
    // full screen with blue color (1)
    memset((char *)0xA0000, 1, (320*200));
     
    for (int i = 0; i < 5; i++) {
        // regs.ax = 0x0000;
        // int32(0x16, &regs);
        timer_wait(500);
        memset((char *)0xA0000, rand(), (320*200));
    }
     
    // switch to 80x25x16 text mode
    regs.ax = 0x0003;
    int32(0x10, &regs);
}