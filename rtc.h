
#ifndef RTC_H
#define RTC_H

#include <stdint.h>

/**
 * Configure RTC0 as a 10ms counter
 */
void init_rtc();

/**
 * Get number of milliseconds since startup
 */
uint32_t get_time();

#endif
