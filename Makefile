
#
# Author: Matthias Bock <mail@matthiasbock.net>
# License: GNU GPLv3
#

 
#
# Toolchain
#
TOOLCHAIN_PATH   = /usr/bin/
TOOLCHAIN_PREFIX = arm-none-eabi
AS      = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-as
CC      = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-gcc
LD      = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-ld
OBJCOPY = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-objcopy
OBJDUMP = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-objdump
SIZE    = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-size
GDB     = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-gdb

newlib  = /usr/lib/arm-none-eabi/newlib/libc.a

#
# Compiler and Linker
#
CFLAGS += -std=gnu99 -Wall -Wextra -g
CFLAGS += -mcpu=cortex-m0 -mthumb -mabi=aapcs -mfloat-abi=soft
CFLAGS += -ffreestanding
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums
CFLAGS += -I arm/
CFLAGS += -I nordic/
CFLAGS += -I sdk/

LDFLAGS += -L /usr/lib/gcc/arm-none-eabi/4.8/armv6-m/
LDFLAGS += -L /usr/lib/arm-none-eabi/newlib/armv6-m/
LDFLAGS += -static
LDFLAGS += -nostartfiles -nostdlib
LDFLAGS += --start-group
LDFLAGS += -lgcc

# required for semihosting 
CFLAGS  += --specs=rdimon.specs
LDFLAGS += -lrdimon 


#
# Build targets
#

all: uart.o delay.o fifo.o nrf51_startup.o pwm.o radio.o timers.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o */*.o *.out *.bin *.elf *.hex *.map

