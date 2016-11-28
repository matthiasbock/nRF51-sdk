/**
 * nRF51822 Power registers
 */

#ifndef POWER_H
#define POWER_H

/*
 * Power registers
 */
#define POWER_BASE   0x4000000

// configure power to RAM banks
#define POWER_RAMON  (*(volatile uint32_t*) (POWER_BASE+0x524))
#define POWER_RAMONB (*(volatile uint32_t*) (POWER_BASE+0x554))

/*
 * Valid values for the above registers
 */
#define POWER_RAMON_ONRAM0 1
#define POWER_RAMON_ONRAM1 2

#define POWER_RAMONB_ONRAM2 1
#define POWER_RAMONB_ONRAM3 2

/*
 * Register macros
 */
#define enable_ram_bank0    POWER_RAMON = POWER_RAMON_ONRAM0
#define enable_ram_bank1    POWER_RAMON = POWER_RAMON_ONRAM1
#define enable_ram_bank2    POWER_RAMONB = POWER_RAMONB_ONRAM2
#define enable_ram_bank3    POWER_RAMONB = POWER_RAMONB_ONRAM3
#define enable_all_ram_banks \
            POWER_RAMON  = POWER_RAMON_ONRAM0  | POWER_RAMON_ONRAM1; \
            POWER_RAMONB = POWER_RAMONB_ONRAM2 | POWER_RAMONB_ONRAM3;

#endif // POWER_H
