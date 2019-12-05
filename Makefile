TARGET = torqi.elf
LINKER_SCRIPT = link.ld
MCU = ATSAMD21E16B

SRC = \
src/boot.S \
src/setup.c \
src/com/debug.c \
src/usb/usbm.c \
src/usb/usb.c \
src/main.c

INC = \
-Iinc/cmsis \
-Iinc/dfp \
-Isrc

PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
SIZE = $(PREFIX)size
CFLAGS =  -mthumb -march=armv6-m -mcpu=cortex-m0plus -nostdlib -Werror
CFLAGS +=  -O2 -D__$(MCU)__ -DDEBUG_MODE

all: $(TARGET)

flash: $(TARGET)
	openocd -f openocd.cfg -c "program $<; reset; exit"

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INC) -T$(LINKER_SCRIPT) -o $@ $^
	$(SIZE) $@
