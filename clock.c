
#include "clock.h"


void init_lfclk()
{
    // Stop LF clock
    lfclk_stop();
    while (lfclk_is_running());

    // Select external 32.786kHz quartz as clock source
    CLOCK_LFCLKSRC = LFCLKSRC_XTAL;

    // Start LF clock
    lfclk_start();
    while (!lfclk_is_running());
}
