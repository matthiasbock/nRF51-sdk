/**
 * Startup code for the nRF51822
 *
 * Required to setup the ARM Cortex M0
 * before any programs can run on it
 *
 * Author: Matthias Bock <mail@matthiasbock.net>
 * License: GNU GPLv3
 */

#include <stdint.h>
#include "cortex_m0.h"

/*
 * Addresses defined in the linker script
 */
extern uint32_t code_end;
extern uint32_t text_end;

extern uint32_t data_begin;
extern uint32_t data_end;

extern uint32_t bss_begin;
extern uint32_t bss_end;

extern uint32_t heap_begin;
extern uint32_t heap_end;

extern uint32_t stack_begin;
extern uint32_t stack_end;


/*
 * Interrupt service routines
 */
 
// invoked upon power-up; must be defined
void Reset_Handler();

// by default all interrupts are serviced by the default handler 
void Default_Handler();

#define WEAK_ALIAS(x)       __attribute__ ((weak, alias(#x)))

void NMI_Handler()          WEAK_ALIAS(Default_Handler);
void HardFault_Handler()    WEAK_ALIAS(Default_Handler);
void MemManage_Handler()    WEAK_ALIAS(Default_Handler);
void BusFault_Handler()     WEAK_ALIAS(Default_Handler);
void UsageFault_Handler()   WEAK_ALIAS(Default_Handler);
void SVC_Handler()          WEAK_ALIAS(Default_Handler);
void DebugMon_Handler()     WEAK_ALIAS(Default_Handler);
void PendSV_Handler()       WEAK_ALIAS(Default_Handler);
void SysTick_Handler()      WEAK_ALIAS(Default_Handler);

// SoC-specific ISRs
void POWER_CLOCK_Handler()  WEAK_ALIAS(Default_Handler);
void RADIO_Handler()        WEAK_ALIAS(Default_Handler);
//extern void uart_isr();
//void UART0_Handler()        WEAK_ALIAS(uart_isr);
void UART0_Handler()        WEAK_ALIAS(Default_Handler);
void SPI0_TWI0_Handler()    WEAK_ALIAS(Default_Handler);
void SPI1_TWI1_Handler()    WEAK_ALIAS(Default_Handler);
void RESERVED()             WEAK_ALIAS(Default_Handler);
void GPIOTE_Handler()       WEAK_ALIAS(Default_Handler);
void ADC_Handler()          WEAK_ALIAS(Default_Handler);
void TIMER0_Handler()       WEAK_ALIAS(Default_Handler);
void TIMER1_Handler()       WEAK_ALIAS(Default_Handler);
void TIMER2_Handler()       WEAK_ALIAS(Default_Handler);
void RTC0_Handler()         WEAK_ALIAS(Default_Handler);
void TEMP_Handler()         WEAK_ALIAS(Default_Handler);
void RNG_Handler()          WEAK_ALIAS(Default_Handler);
void ECB_Handler()          WEAK_ALIAS(Default_Handler);
void CCM_AAR_Handler()      WEAK_ALIAS(Default_Handler);
void WDT_Handler()          WEAK_ALIAS(Default_Handler);
void RTC1_Handler()         WEAK_ALIAS(Default_Handler);
void QDEC_Handler()         WEAK_ALIAS(Default_Handler);
void LPCOMP_Handler()       WEAK_ALIAS(Default_Handler);
void SWI0_Handler()         WEAK_ALIAS(Default_Handler);
void SWI1_Handler()         WEAK_ALIAS(Default_Handler);
void SWI2_Handler()         WEAK_ALIAS(Default_Handler);
void SWI3_Handler()         WEAK_ALIAS(Default_Handler);
void SWI4_Handler()         WEAK_ALIAS(Default_Handler);
void SWI5_Handler()         WEAK_ALIAS(Default_Handler);

#define RESERVED 0

/*
 * Exception vector table
 * http://community.arm.com/docs/DOC-8769
 */
void *vectors[] __attribute__ ((section(".vectors"))) =
{
    &stack_end,

    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    RESERVED,
    RESERVED,
    RESERVED,
    RESERVED,
    SVC_Handler,
    DebugMon_Handler,
    RESERVED,
    PendSV_Handler,
    SysTick_Handler,

    POWER_CLOCK_Handler,
    RADIO_Handler,
    UART0_Handler,
    SPI0_TWI0_Handler,
    SPI1_TWI1_Handler,
    RESERVED,
    GPIOTE_Handler,
    ADC_Handler,
    TIMER0_Handler,
    TIMER1_Handler,
    TIMER2_Handler,
    RTC0_Handler,
    TEMP_Handler,
    RNG_Handler,
    ECB_Handler,
    CCM_AAR_Handler,
    WDT_Handler,
    RTC1_Handler,
    QDEC_Handler,
    LPCOMP_Handler,
    SWI0_Handler,
    SWI1_Handler,
    SWI2_Handler,
    SWI3_Handler,
    SWI4_Handler,
    SWI5_Handler,
    RESERVED,
    RESERVED,
    RESERVED,
    RESERVED,
    RESERVED,
    RESERVED
};

// Reset_Handler() jumps to main() when finished
extern int main();

/*
 * This code is the first to execute
 * when the chip is powering up or reset
 */
void Reset_Handler()
{
    /*
     * Copy non-constant variables
     * from flash memory (behind code)
     * to RAM (data section)
     */
    uint32_t *src;
    uint32_t *dst;
    src = &text_end;
    dst = &data_begin;
    while (dst <= &data_end)
        *(dst++) = *(src++);

    // clear the bss section: initialize uninitialized variables with zeros
    // clear the heap section: memory shall be filled with zeros after malloc
    dst = &bss_begin;
    while (dst < &heap_end)
        *(dst++) = 0;

    // the stack does not need to be filled with zeros,
    // as all pop'ed values will necessarily have previously been push'ed

    // all initialization is done, jump to main function
    main();
    
    // this point should never be reached
    // TODO: go to deep sleep or reset
}

/*
 * Default interrupt service routine:
 * In case of an unhandled interrupt, go to sleep.
 */
void Default_Handler()
{
    // mentioned here for (hard-)fault debugging:
    uint32_t icsr = ICSR;
    uint32_t interrupt_active = INTACTIVE;
    uint32_t interrupt_pending = INTPENDING;
}
