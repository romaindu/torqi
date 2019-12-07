#ifndef USB_H
#define USB_H

#include <stdint.h>
#include <string.h>

#include "com/serial.h"
#include "descriptors.h"
#include "samd21.h"

#define USB_NUM_ENDPOINTS   2

typedef enum {
    EP_DISABLED,
    EP_CONTROL,
    EP_ISOCHRONOUS,
    EP_BULK,
    EP_INTERRUPT,
} EpType_t;

typedef struct {
    uint8_t  bmRequestType;
    uint8_t  bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
} __attribute__((packed, aligned(1))) SetupPacket_t;

typedef struct {
    const void  *addr;
    int16_t     idx;
    int16_t     len;
} PacketXfer_t;

typedef struct {

} ControlTransfer_t;

#define IS_STANDARD_REQUEST(x)   (!(x & 0x60))
enum {
    GET_STATUS = 0,
    CLEAR_FEATURE = 1,
    SET_FEATURE = 3,
    SET_ADDRESS = 5,
    GET_DESCRIPTOR = 6,
    GET_CONFIGURATION = 8,
    SET_CONFIGURATION = 9,
};

/* Hardware module functions (usbm.c) */
void usbm_init(void);
void usbm_attach(void);
void usbm_poll(void);
void usbm_configure_ep(uint8_t, uint8_t, EpType_t, void *);
void usbm_ep_clr_out(uint8_t);
void usbm_ep_stall(uint8_t);
void usbm_ep_send_in(uint8_t, int16_t);
void usbm_set_address(uint8_t);

/* Protocol related functions (usb.c) */
void usb_on_reset(void);
void usb_on_setup_request(void);
void usb_on_in_xfer(uint8_t);
void usb_on_out_xfer(uint8_t, uint8_t);
void usb_set_configuration(uint16_t);

#endif