#pragma once
#include "../io/io.h"
#include <stdint.h>

extern uint8_t rtc_second;
extern uint8_t rtc_minute;
extern uint8_t rtc_hour;
extern uint8_t rtc_day;
extern uint8_t rtc_month;
extern uint8_t rtc_year;
void read_rtc();