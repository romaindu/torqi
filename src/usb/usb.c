/*
 * usb.c
 *
 * @author: Romain Durand
*/

#include "usb.h"

#include <string.h>
#include "com/serial.h"
#include "descriptors.h"
#include "hid.h"

#define MIN(a, b)   ((a < b)?(a):(b))
#define IS_STANDARD_REQUEST(x)   (!((x) & 0x60))

enum {
    GET_STATUS = 0,
    CLEAR_FEATURE = 1,
    SET_FEATURE = 3,
    SET_ADDRESS = 5,
    GET_DESCRIPTOR = 6,
    GET_CONFIGURATION = 8,
    SET_CONFIGURATION = 9,
};

static SetupPacket_t setupPacket;
static PacketXfer_t  controlXfer;
static uint8_t epZeroInBuf[USB_EP0_SIZE];
static uint8_t epZeroOutBuf[USB_EP0_SIZE];
static uint16_t usbConfiguration = 0;

RequestedDesc_t const * descriptor_get_requested(
    uint16_t wValue, uint16_t wIndex)
{
    int i = 0;

    do {
        if (reqDescList[i].wValue == wValue &&
                reqDescList[i].wIndex == wIndex) {
            return &reqDescList[i]; 
        }
    } while (++i < DESCRIPTORS_COUNT);

    return 0;
}

void ep0_send_next_packet(void)
{
    uint8_t bc;
    int16_t rem;

    rem = controlXfer.len - controlXfer.idx;

    if (rem > 0) {
        /* There is still data to send */
        bc = MIN(USB_EP0_SIZE, rem);
        memcpy(epZeroInBuf, controlXfer.addr + controlXfer.idx, bc);
        usbm_ep_send_in(0x80, bc);
        controlXfer.idx += USB_EP0_SIZE;
    }
    else {
        if (rem == 0) {
            /* The data was a multiple of the endpoint size -> send ZLP */
            usbm_ep_send_in(0x80, 0);
        }
        /* This packet is over */
        usbm_ep_clr_out(0x00);
    }
}

void usb_on_reset(void)
{
    memset(&setupPacket, 0, sizeof(SetupPacket_t));
    memset(&controlXfer, 0, sizeof(PacketXfer_t));
    usbm_configure_ep(0x00, USB_EP0_SIZE, EP_CONTROL, epZeroOutBuf);
    usbm_configure_ep(0x80, USB_EP0_SIZE, EP_CONTROL, epZeroInBuf);
}

void usb_on_setup_request(void)
{
    uint16_t wValue, wIndex, wLength;
    uint8_t bc;
    RequestedDesc_t const * reqDesc;

    memcpy(&setupPacket, epZeroOutBuf, sizeof(SetupPacket_t));
    putm(&setupPacket, sizeof(SetupPacket_t));

    wValue = setupPacket.wValue;
    wIndex = setupPacket.wIndex;
    wLength = setupPacket.wLength;

    if (IS_STANDARD_REQUEST(setupPacket.bmRequestType)) {
        switch (setupPacket.bRequest) {
            case GET_STATUS:
                puts("USB: GET_STATUS\n");
                epZeroInBuf[0] = 0;
                epZeroInBuf[1] = 0;
                usbm_ep_send_in(0x80, 2);
                usbm_ep_clr_out(0x00);
                break;
            case SET_ADDRESS:
                puts("USB: SET_ADDRESS\n");
                usbm_ep_send_in(0x80, 0);
                usbm_ep_clr_out(0x00);
                break;
            case GET_DESCRIPTOR:
                puts("USB: GET_DESCRIPTOR\n");
                reqDesc = descriptor_get_requested(wValue, wIndex);
                if (reqDesc) {
                    controlXfer.addr = reqDesc->addr;
                    controlXfer.idx = 0;
                    controlXfer.len = MIN(wLength, reqDesc->len);
                    ep0_send_next_packet();
                } else {
                    puts("USB: ERROR: Descriptor not found.\n");
                    usbm_ep_stall(0x80);
                }
                break;
            case GET_CONFIGURATION:
                puts("USB: GET_CONFIGURATION\n");
                epZeroInBuf[0] = usbConfiguration;
                usbm_ep_send_in(0x80, 1);
                usbm_ep_clr_out(0x00);
                break;
            case SET_CONFIGURATION:
                puts("USB: SET_CONFIGURATION\n");
                usbConfiguration = wValue;
                hid_configure(wValue);
                usbm_ep_send_in(0x80, 0);
                usbm_ep_clr_out(0x00);
                break;
            default:
                puts("USB: ERROR: UNHANDLED STANDARD REQUEST.\n");
                usbm_ep_stall(0x00);
                usbm_ep_stall(0x80);
                break;
        }
    }
    else {
        hid_on_setup_request(setupPacket);
    }
}

void usb_on_in_xfer(uint8_t ep)
{
    if (ep == 0x80 && IS_STANDARD_REQUEST(setupPacket.bmRequestType)) {
        switch (setupPacket.bRequest) {
            case SET_ADDRESS:
                usbm_set_address(setupPacket.wValue);
                break;
            case GET_DESCRIPTOR:
                ep0_send_next_packet();
                break;
        }
    }
    else {
        hid_on_in_xfer(ep);
    }
}

void usb_on_out_xfer(uint8_t ep, uint8_t bc)
{
    hid_on_out_xfer(ep, bc);
}
