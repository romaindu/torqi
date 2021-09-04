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
lib/tinyusb/portable/microchip/samd/dcd_samd.c \
lib/tinyusb/tusb.c \
\
src/mot/torqi.c \
src/mot/motor.c \
src/whl/wheel.c \
src/ffb/effects.c \
src/ffb/ffb.c \
src/usb/descriptors.c \
src/usb/requests.c \
src/util.c \
src/main.c \

INC = \
-Iinc/cmsis \
-Iinc/dfp \
-Ilib/tinyusb \
-Isrc

PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
LD = $(PREFIX)ld
AR = $(PREFIX)ar
SIZE = $(PREFIX)size
CFLAGS =  -mthumb -march=armv6-m -mcpu=cortex-m0plus -nostartfiles -Werror
CFLAGS += -O1 -D__$(MCU)__ -DDEBUG_MODE -DPRINTF_INCLUDE_CONFIG_H

all: $(TARGET)

.PHONY: clean
clean:
	rm -rf $(TARGET)

flash: $(TARGET)
	openocd -f openocd.cfg -c "program $<; reset; exit"

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INC) -T$(LINKER_SCRIPT) -o $@ $^
	$(SIZE) $@
