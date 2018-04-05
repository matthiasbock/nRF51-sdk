
/*
 * GCC linker script for the nRF51
 *
 * Author: Matthias Bock <mail@matthiasbock.net>
 * License: GNU GPLv3
 */

/* Code execution starts in the Reset_Handler() */ 
ENTRY(Reset_Handler)

/*
 * Configure memory regions
 *
 * According to section 3.2 of the nRF51822 datasheet,
 * the QFAC variant of the chip has
 *     256kB Flash memory in 256 pages of 1024 bytes
 *  and 32kB RAM in 4 blocks of 8kB.
 */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 256k
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 32k
}

heap_size = 512;

INCLUDE nrf51-common.ld
