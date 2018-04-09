
#include "rtc.h"
#include "clock.h"
#include <nrf51.h>


void init_rtc()
{
    // Start LF clock, if necessary
    if (!lfclk_is_running())
    {
        init_lfclk();
    }

    // Stop RTC in case it is already running
    NRF_RTC0->TASKS_STOP = 1;
    NRF_RTC0->TASKS_CLEAR = 1;

    // Set prescaler to approximate a resolution of 10ms
    NRF_RTC0->PRESCALER = 327;

    // Start
    NRF_RTC0->TASKS_START = 1;
}


uint32_t get_time()
{
    // 1 increment corresponds to about 10ms
    return NRF_RTC0->COUNTER * 10;
}

