CROSS ?= arm-none-eabi-
CC = $(CROSS)gcc
LD = $(CROSS)ld
AR = $(CROSS)ar
OBJCOPY = $(CROSS)objcopy
INCLUDES = -I../include -I../../include
CFLAGS = -nostdlib -mlittle-endian -mcpu=cortex-a8 -mthumb -mthumb-interwork -D_N81AP_=1 -Werror -Os -mfpu=vfp
.SILENT:

%.o:	%.S
	@echo ASSEMBLE $(@)
	@$(CC) $(INCLUDES) $(CFLAGS) -c $(<) -o $(@)

%.o:	%.c
	@echo CC $(@)
	@$(CC) $(INCLUDES) $(CFLAGS) -c $(<) -o $(@)


