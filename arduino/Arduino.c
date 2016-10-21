/**
 * Wrapper for Arduino functions to nRF51 SDK functions
 */

#include "Arduino.h"

/**
 * Setting pin mode of operation
 */
void pinMode(uint8_t pin, uint8_t mode)
{
    if (mode == OUTPUT)
    {
        nrf_gpio_cfg_output(pin);
    }
    if (mode == INPUT)
    {
        nrf_gpio_cfg_input(pin, NRF_GPIO_PIN_NOPULL);
    }
}

/**
 * Write to GPIO pin
 */
void digitalWrite(uint8_t pin, uint8_t level)
{
    nrf_gpio_pin_write(pin, level);
}

/**
 * Read from GPIO pin
 */
int digitalRead(uint8_t pin)
{
    return nrf_gpio_pin_read(pin);
}

int analogRead(uint8_t pin)
{
    return digitalRead(pin);
}

void analogWrite(uint8_t pin, int level)
{
    digitalWrite(pin, level);
}

void delay(unsigned long ms)
{
    nrf_delay_ms(ms);
}

void delayMicroseconds(unsigned int us)
{
    for (uint32_t c=0; c<us; c++)
    {
        asm("nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;");
    }
}
