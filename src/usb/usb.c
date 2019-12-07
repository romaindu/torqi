/*
 * usb.c
 *
 * @author: Romain Durand
*/

#include "usb.h"

#define MIN(a, b)   ((a < b)?(a):(b))

SetupPacket_t setupPacket;
PacketXfer_t  controlXfer;
uint8_t epZeroInBuf[USB_EP0_SIZE];
uint8_t epZeroOutBuf[USB_EP0_SIZE];
uint16_t usbConfiguration = 0;

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
        usbm_ep_send_in(0, bc);
        controlXfer.idx += USB_EP0_SIZE;
    }
    else {
        if (rem == 0) {
            /* The data was a multiple of the endpoint size -> send ZLP */
            usbm_ep_send_in(0, 0);
        }
        /* This packet is over */
        usbm_ep_clr_out(0);
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
                puts("GET_STATUS\n");
                break;
            case CLEAR_FEATURE:
                puts("CLEAR_FEATURE\n");
                break;
            case SET_FEATURE:
                puts("SET_FEATURE\n");
                break;
            case SET_ADDRESS:
                puts("SET_ADDRESS\n");
                usbm_ep_send_in(0, 0);
                usbm_ep_clr_out(0);
                break;
            case GET_DESCRIPTOR:
                puts("GET_DESCRIPTOR\n");
                reqDesc = descriptor_get_requested(wValue, wIndex);
                if (reqDesc) {
                    controlXfer.addr = reqDesc->addr;
                    controlXfer.idx = 0;
                    controlXfer.len = MIN(wLength, reqDesc->len);
                    ep0_send_next_packet();
                } else {
                    puts("ERROR: No descriptor found.\n");
                    usbm_ep_stall(0x80);
                }
                break;
            case GET_CONFIGURATION:
                puts("GET_CONFIGURATION\n");
                break;
            case SET_CONFIGURATION:
                puts("SET_CONFIGURATION\n");
                usb_set_configuration(wValue);
                usbm_ep_send_in(0, 0);
                usbm_ep_clr_out(0);
                break;
            default:
                puts("ERROR: UNHANDLED STANDARD REQUEST.\n");
                usbm_ep_clr_out(0);
                break;
        }
    }
    else {
        puts("ERROR: NON STANDARD REQUEST.\n");
        usbm_ep_clr_out(0);
        usbm_ep_send_in(0, 0);
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
}

void usb_on_out_xfer(uint8_t ep, uint8_t bc)
{

}
