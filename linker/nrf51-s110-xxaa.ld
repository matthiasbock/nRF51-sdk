
/*
 * GCC linker script for the nRF51
 * when Nordic's SoftDevice is being used
 *
 * Author: Matthias Bock <mail@matthiasbock.net>
 * License: GNU GPLv3
 */

/* Code execution starts in the Reset_Handler() */ 
ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x00016000, LENGTH = 186k
  RAM   (rwx) : ORIGIN = 0x20002000, LENGTH = 8k
}

heap_size = 512;

INCLUDE nrf51-common.ld
