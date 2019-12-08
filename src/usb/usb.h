#ifndef USB_H
#define USB_H

#include <stdint.h>

/* Affects the RAM usage, max 8 (device limited) */
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

/* Hardware module functions (usbm.c) */
void usbm_init(void);
void usbm_attach(void);
void usbm_poll(void);
void usbm_configure_ep(uint8_t ep, uint8_t size, EpType_t type, void *buf);
void usbm_ep_clr_out(uint8_t ep);
void usbm_ep_stall(uint8_t ep);
void usbm_ep_send_in(uint8_t ep, int16_t bc);
void usbm_set_address(uint8_t addr);

/* Protocol related functions (usb.c) */
void usb_on_reset(void);
void usb_on_setup_request(void);
void usb_on_in_xfer(uint8_t ep);
void usb_on_out_xfer(uint8_t ep, uint8_t bc);

#endif