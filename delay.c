
/**
 * Delay routines library
 * for Cortex M0-based embedded systems running at 16 MHz
 *
 * Author:  Matthias Bock <mail@matthiasbock.net>
 * License: GNU GPLv3
 */

#include "delay.h" 

void delay_us(uint32_t us)
{
    // CPU is operating at 16 MHz:
    // 16 instructions take 1 us
    asm volatile (
        "start%=: nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "sub %[cycles], #1\n\t"
        "bne start%=\n\t"
        : /* empty */ : [cycles] "l" (us)
    );
}

// TODO: something's broken with this function, it doesn't work anymore...
void delay_ms(uint32_t ms)
{
    // TODO: use Timer instead of NOPs for longer timespans,
    //       so we can shut down CPU and save some power
    for (uint32_t i=0; i<ms; i++)
        delay_us(999);
}
