#include "random.h"

unsigned int seed = 1;

void srandt() {
    read_rtc();
    seed = rtc_second + rtc_minute + rtc_hour + rtc_day + rtc_month + rtc_year;
}

int rand() {
    seed = (seed * 1103515245U + 12345U) & 0x7fffffffU;
    return (int)seed;
}