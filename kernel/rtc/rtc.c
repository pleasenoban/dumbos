#include "rtc.h"

#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

uint8_t rtc_second = 0;
uint8_t rtc_minute = 0;
uint8_t rtc_hour = 0;
uint8_t rtc_day = 0;
uint8_t rtc_month = 0;
uint8_t rtc_year = 0;

int get_update_in_progress_flag() {
    outb(CMOS_ADDRESS, 0x0A);
    return (inb(CMOS_DATA) & 0x80);
}

uint8_t get_RTC_register(int reg) {
    outb(CMOS_ADDRESS, reg);
    return inb(CMOS_DATA);
}

void read_rtc() {
    // we dont really need accuracy here
    while (get_update_in_progress_flag());
    rtc_second = get_RTC_register(0x00);
    rtc_minute = get_RTC_register(0x02);
    rtc_hour = get_RTC_register(0x04);
    rtc_day = get_RTC_register(0x07);
    rtc_month = get_RTC_register(0x08);
    rtc_year = get_RTC_register(0x09);
}