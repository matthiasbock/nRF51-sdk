
/**
 * UART library
 * for the Nordic Semiconductor nRF51 series
 *
 * Author: Matthias Bock <mail@matthiasbock.net>
 * License: GNU GPLv3
 */

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"
#include "delay.h"
#include "strings.h"

#ifdef UART_USE_FIFO
#include "fifo.h"
#endif

/*
 * Registers of the UART peripheral
 */

#define UART_BASE   0x40002000

// Tasks
#define UART_TASK_STARTRX     (*(volatile uint32_t*) (UART_BASE+0x000))   // Start UART receiver
#define UART_TASK_STOPRX      (*(volatile uint32_t*) (UART_BASE+0x004))   // Stop UART receiver
#define UART_TASK_STARTTX     (*(volatile uint32_t*) (UART_BASE+0x008))   // Start UART transmitter
#define UART_TASK_STOPTX      (*(volatile uint32_t*) (UART_BASE+0x00C))   // Stop UART transmitter
#define UART_TASK_SUSPEND     (*(volatile uint32_t*) (UART_BASE+0x01C))   // Suspend UART

// Events
#define UART_EVENT_CTS        (*(volatile uint32_t*) (UART_BASE+0x100))   // CTS is activated (set low)
#define UART_EVENT_NCTS       (*(volatile uint32_t*) (UART_BASE+0x104))   // CTS is deactivated (set high)
#define UART_EVENT_RXDRDY     (*(volatile uint32_t*) (UART_BASE+0x108))   // Data received in RXD
#define UART_EVENT_TXDRDY     (*(volatile uint32_t*) (UART_BASE+0x11C))   // Data sent from TXD
#define UART_EVENT_ERROR      (*(volatile uint32_t*) (UART_BASE+0x124))   // Error detected
#define UART_EVENT_RXTO       (*(volatile uint32_t*) (UART_BASE+0x144))   // Receiver timeout

// Registers
#define UART_INTEN       (*(volatile uint32_t*) (UART_BASE+0x300))   // Enable or disable interrupt
#define UART_INTENSET    (*(volatile uint32_t*) (UART_BASE+0x304))   // Enable interrupt
#define UART_INTENCLR    (*(volatile uint32_t*) (UART_BASE+0x308))   // Disable interrupt
#define UART_ERRORSRC    (*(volatile uint32_t*) (UART_BASE+0x480))   // Error source
#define UART_ENABLE      (*(volatile uint32_t*) (UART_BASE+0x500))   // Enable UART
#define UART_PSELRTS     (*(volatile uint32_t*) (UART_BASE+0x508))   // Pin select for RTS
#define UART_PSELTXD     (*(volatile uint32_t*) (UART_BASE+0x50C))   // Pin select for TXD
#define UART_PSELCTS     (*(volatile uint32_t*) (UART_BASE+0x510))   // Pin select for CTS
#define UART_PSELRXD     (*(volatile uint32_t*) (UART_BASE+0x514))   // Pin select for RXD
#define UART_RXD         (*(volatile uint32_t*) (UART_BASE+0x518))   // RXD register
#define UART_TXD         (*(volatile uint32_t*) (UART_BASE+0x51C))   // TXD register
#define UART_BAUDRATE    (*(volatile uint32_t*) (UART_BASE+0x524))   // Baud rate
#define UART_CONFIG      (*(volatile uint32_t*) (UART_BASE+0x56C))   // Configuration of parity and hardware flow control


// Valid baudrates
#define UART_BAUD_1200    0x0004F000
#define UART_BAUD_2400    0x0009D000
#define UART_BAUD_4800    0x0013B000
#define UART_BAUD_9600    0x00275000
#define UART_BAUD_14400   0x003B0000
#define UART_BAUD_19200   0x004EA000
#define UART_BAUD_28800   0x0075F000
#define UART_BAUD_38400   0x009D5000
#define UART_BAUD_57600   0x00EBF000
#define UART_BAUD_76800   0x013A9000
#define UART_BAUD_115200  0x01D7E000
#define UART_BAUD_230400  0x03AFB000
#define UART_BAUD_250000  0x04000000
#define UART_BAUD_460800  0x075F7000
#define UART_BAUD_921600  0x0EBED000
#define UART_BAUD_1M      0x10000000


/*
 * Macros for writing data to registers
 */

// Task macros
#define uart_start_receiver()           UART_TASK_STARTRX = 1
#define uart_stop_receiver()            UART_TASK_STOPRX  = 1
#define uart_start_transmitter()        UART_TASK_STARTTX = 1
#define uart_stop_transmitter()         UART_TASK_STOPTX  = 1
#define uart_stop_transmitter()         UART_TASK_STOPTX  = 1

// write a zero to clear the event
#define clear_event(event)              event = 0

// Interrupts
#define UART_INTERRUPT                          2
#define UART_ISR_VECTOR_ADDRESS                 (16 + UART_INTERRUPT)*4
// that's not working:
//#define set_uart_interrupt_handler(pointer)     *(volatile uint32_t*) (UART_ISR_VECTOR_ADDRESS) = (uint32_t) pointer // +1 to indicate Thumb instruction set

#define uart_interrupt_enable()                 interrupt_enable(UART_INTERRUPT)
#define uart_interrupt_disable()                interrupt_disable(UART_INTERRUPT)

#define uart_interrupt_upon_RXDRDY_enable()     UART_INTENSET = (1 << 2)
#define uart_interrupt_upon_RXDRDY_disable()    UART_INTENCLR = (1 << 2)

#define uart_interrupt_upon_TXDRDY_enable()     UART_INTENSET = (1 << 7)
#define uart_interrupt_upon_TXDRDY_disable()    UART_INTENCLR = (1 << 7)

#define uart_interrupt_upon_ERROR_enable()      UART_INTENSET = (1 << 9)
#define uart_interrupt_upon_ERROR_disable()     UART_INTENCLR = (1 << 9)

#define uart_interrupt_upon_RXTO_enable()       UART_INTENSET = (1 << 17)
#define uart_interrupt_upon_RXTO_disable()      UART_INTENCLR = (1 << 17)

// Configuration
#define uart_disable()                  UART_ENABLE =  UART_ENABLE & 0xFFFFFFF8
#define uart_enable()                   UART_ENABLE = (UART_ENABLE & 0xFFFFFFF8) + 0x4

// only configure pins while UART is disabled
#define uart_select_pin_as_TXD(pin)     UART_PSELTXD = pin
#define uart_select_pin_as_RXD(pin)     UART_PSELRXD = pin
#define uart_select_pin_as_RTS(pin)     UART_PSELRTS = pin
#define uart_select_pin_as_CTS(pin)     UART_PSELCTS = pin

// signal shall not be externalized to any pin
#define UART_PIN_DISABLE                0xFFFFFFFF

#define uart_read()                    (UART_RXD & 0x000000FF)
#define uart_write(b)                   UART_TXD = b

#define uart_set_baud(rate)             UART_BAUDRATE = rate

#define uart_set_parity_exclude()       UART_CONFIG =  UART_CONFIG & 0xFFFFFFF1
#define uart_set_parity_include()       UART_CONFIG = (UART_CONFIG & 0xFFFFFFF1) + 0xE

#define uart_flow_control_disable()     UART_CONFIG =  UART_CONFIG & 0xFFFFFFFE
#define uart_flow_control_enable()      UART_CONFIG = (UART_CONFIG & 0xFFFFFFFE) + 0x1


/*
 * Functions provided by this library
 */
void    uart_init(
            uint32_t pin_rx,
            uint32_t pin_tx,
            uint32_t pin_rts,
            uint32_t pin_cts,
            uint32_t baud,
            bool parity,
            bool flowcontrol
            );
void    uart_send_char(char c);
void    uart_send_bytes(char* s, uint8_t length);
void    uart_send_string(char* s);
void    uart_receive_char(char* c);
void    uart_receive_line(char* line, uint8_t* length);

/*
 * To use a FIFO buffer,
 * define one and add #define UART_USE_FIFO
 * in your code
 */
#ifdef UART_USE_FIFO

// TODO: implementation is incomplete:

void    uart_fifo_init(fifo_t* outfifo, fifo_t* infifo);
void    uart_fifo_send_char(fifo_t* outfifo, char c);
void    uart_fifo_send_buffer(fifo_t* outfifo, char* buffer, uint8_t length);
void    uart_fifo_send_string(fifo_t* outfifo, char* s);
void    uart_fifo_receive_char(fifo_t* infifo, char* c);
void    uart_fifo_receive_line(fifo_t* infifo, char* line, uint8_t* length);
#endif

#endif // UART_H
