TARGET = torqi.elf
LINKER_SCRIPT = link.ld
MCU = ATSAMD21E16B

SRC = \
src/boot.S \
\
lib/tinyusb/class/hid/hid_device.c \
lib/tinyusb/common/tusb_fifo.c \
lib/tinyusb/device/usbd.c \
lib/tinyusb/device/usbd_control.c \
lib/tinyusb/portable/microchip/samd21/dcd_samd21.c \
lib/tinyusb/tusb.c \
\
lib/printf/printf.c \
\
src/com/debug.c \
src/usb/descriptors.c \
src/usb/requests.c \
src/ffb/ffb.c \
src/ffb/effects.c \
src/mot/motor.c \
src/mot/controller.c \
src/mot/torque.c \
src/util.c \
src/main.c

INC = \
-Iinc/cmsis \
-Iinc/dfp \
-Ilib/tinyusb \
-Ilib/printf \
-Isrc/com \
-Isrc/usb \
-Isrc/mot \
-Isrc/ffb \
-Isrc

PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
SIZE = $(PREFIX)size
CFLAGS =  -mthumb -march=armv6-m -mcpu=cortex-m0plus -nostartfiles -Werror 
CFLAGS += -O1 -D__$(MCU)__ -DDEBUG_MODE -DPRINTF_INCLUDE_CONFIG_H

all: $(TARGET)

flash: $(TARGET)
	openocd -f openocd.cfg -c "program $<; reset; exit"

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INC) -T$(LINKER_SCRIPT) -o $@ $^
	$(SIZE) $@
