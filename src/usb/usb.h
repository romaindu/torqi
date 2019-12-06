#ifndef USB_H
#define USB_H

#include <stdint.h>
#include <string.h>

#include "com/serial.h"
#include "descriptors.h"
#include "samd21.h"

#define USB_NUM_ENDPOINTS   2

typedef struct {
    uint8_t  bmRequestType;
    uint8_t  bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
} SetupPacket_t;

extern SetupPacket_t setupPacket;
extern uint8_t epZeroInBuf[USB_EP0_SIZE];
extern uint8_t epZeroOutBuf[USB_EP0_SIZE];

/* Hardware module functions (usbm.c) */
void usb_init(void);
void usb_attach(void);
void usb_poll(void);

/* Protocol related functions (usb.c) */

#endif