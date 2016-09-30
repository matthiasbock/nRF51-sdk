
/**
 * UART library
 * for the Nordic Semiconductor nRF51 series
 *
 * Contains methods for operation both with and without usage of a FIFO buffer
 *
 * Author: Matthias Bock <mail@matthiasbock.net>
 * License: GNU GPLv3
 */

#include "uart.h"

void uart_init(
        uint8_t pin_rx,
        uint8_t pin_tx,
        uint8_t pin_rts,
        uint8_t pin_cts,
        uint32_t baud,
        bool parity,
        bool flowcontrol
        )
{
    // configure pins and pin directions

    gpio_config_input(pin_rx);
    uart_select_pin_as_RXD(pin_rx);

    gpio_config_output(pin_tx);
    uart_select_pin_as_TXD(pin_tx);

    uart_flow_control_disable();
    if (flowcontrol)
    {
        gpio_config_output(pin_rts);
        uart_select_pin_as_RTS(pin_rts);

        gpio_config_input(pin_cts);
        uart_select_pin_as_CTS(pin_cts);

        uart_flow_control_enable();
    }

    // set port parameters
    uart_set_baud(baud);

    // set transmitter to "ready"
    UART_EVENT_TXDRDY = 1;

    uart_enable();
}

#ifndef UART_USE_FIFO
void uart_send_char(char* c)
{
    // peripheral switches register UART_EVENT_TXDRDY to 1, when transmission is complete
    while (UART_EVENT_TXDRDY != 1)
    {
        delay_us(18);
    }
    UART_EVENT_TXDRDY = 0;

    uart_write((uint32_t) (*c));
}
#endif

#ifdef UART_USE_FIFO

// we need to know, whether we are already transmitting or not
volatile bool uart_transmitting = false;

/*
 * Initialize FIFO for buffered operation
 */
void uart_fifo_init()
{
    uart_rx_fifo = &uart_inbuffer;
    fifo_init(uart_rx_fifo);

    uart_tx_fifo = &uart_outbuffer;
    fifo_init(uart_tx_fifo);
}

/*
 * UART Interrupt Service Routine
 * attached in nrf51_startup.c
 * to be invoked by NVIC
 */
void UART0_Handler()
{
    // cache event stati for processing
    uint32_t transmitted    = uart_event_TXDRDY;
    uint32_t received       = uart_event_RXDRDY;
    uint32_t timeout        = uart_event_RXTO;
    uint32_t error          = uart_event_ERROR;

    // is the transmitter circuit ready for another byte?
    if (transmitted)
    {
        // send another byte
        if (fifo_available(uart_tx_fifo) > 0)
        {
            char outgoing;
            fifo_read(uart_tx_fifo, &outgoing);
            uart_write(outgoing);
        }
        else // TX buffer is empty
        {
            uart_stop_transmitter;
            uart_transmitting = false;
            // disable future interrupts on TXDRDY event
            uart_interrupt_upon_TXDRDY_disable;
        }

        // clear current TRANSMITTER READY event
        clear_event(uart_event_TXDRDY);
    }

    // did the receiver circuit receive a byte?
    if (received)
    {
        if (!fifo_full(uart_rx_fifo))
        {
            // must be cleared before reading RX,
            // see nRF51 Series Reference Manual p.153
            clear_event(uart_event_RXDRDY);

            // receive one byte
            char incoming = uart_read;

            // echo read byte back to sender
            uart_write(incoming);

            // push received byte to buffer
            fifo_write(uart_rx_fifo, &incoming);
        }
    }
    
    // unhandled, but must be cleared
    if (timeout)
    {
        clear_event(uart_event_RXTO);
    }

    // unhandled, but must be cleared
    if (error)
    {
        clear_event(uart_event_ERROR);
    }
}

/*
 * Send a string to the configured UART
 * by providing a pointer to the buffer
 * and the length of bytes to send
 */
void uart_send(char* buffer, uint8_t length)
{
    // copy data to internal buffer to prevent buffer changes during transmission
    uint8_t timeout = 10;
    for (uint8_t i=0; i<length; i++)
    {
        // FIFO is full? -> wait until timeout
        while (fifo_full(uart_tx_fifo) && (timeout--) > 0)
            delay_us(100);
        // timeout
        if (timeout <= 0)
            break;
        // push current buffer char to FIFO
        DINT;
        fifo_write(uart_tx_fifo, buffer+i);
        if (*(buffer+i) == '\n')
        {
            while (fifo_full(uart_tx_fifo) && (timeout--) > 0)
            {
                EINT;
                delay_us(100);
                DINT;
            }
            if (timeout <= 0)
            {
                EINT;
                break;
            }
            fifo_write(uart_tx_fifo, (char*) &"\r");
        }
        EINT;
    }

    // no need to interrupt ongoing transmissions
    if (!uart_transmitting)
    {
        // set as early as possible, so that interrupts can know about it
        uart_transmitting = true;

        // enable TRANSMITTER READY event interrupt
        uart_interrupt_upon_TXDRDY_enable;

        // enable transmission
        uart_start_transmitter;

        // initiate transmission cycle by writing the first byte to transmitter buffer
        char outgoing;
        fifo_read(uart_tx_fifo, &outgoing);
        uart_write(outgoing);
    }
}

/*
 * Send a string: automatically determine the number of characters to send
 */
void uart_send_string(char* s)
{
    uart_send(s, strlen(s));
}

/*
 * Receive bytes from configured UART
 * by providing a pointer to malloc'ed memory
 * and the maximum number of bytes that may
 * be written to it.
 *
 * Returns the actual number of bytes written to the buffer.  
 */
uint8_t uart_receive(char* buffer, uint8_t max)
{
    return 0;
}

#endif // UART_USE_FIFO
