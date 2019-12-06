/*
 * usbm.c
 *
 * @author: Romain Durand
*/

#include "usb.h"

UsbDeviceDescriptor USBEP[USB_NUM_ENDPOINTS];

void usb_init(void)
{
    USB->DEVICE.CTRLA.bit.SWRST = 1;
    while (USB->DEVICE.SYNCBUSY.bit.SWRST);

    USB->DEVICE.CTRLA.reg = USB_CTRLA_MODE_DEVICE + USB_CTRLA_ENABLE;
    while (USB->DEVICE.SYNCBUSY.bit.ENABLE);

    USB->DEVICE.DESCADD.reg = (uint32_t)&USBEP;
}

void usb_attach(void)
{
    USB->DEVICE.CTRLB.bit.DETACH = 0;
}

static inline int pcksize_calc(uint8_t s)
{
    if (s <= 8)     return 0;
    if (s <= 16)    return 1;
    if (s <= 32)    return 2;
    if (s <= 64)    return 3;
    if (s <= 128)   return 4;
    else            return 5;
}

void usb_configure_ep(
    uint8_t ep,
    uint8_t size,
    EpType_t type,
    void *buf)
{
    if (ep & 0x80) {
        ep &= 0x7;
        USBEP[ep].DeviceDescBank[1].ADDR.reg = (uint32_t)buf;
        USBEP[ep].DeviceDescBank[1].PCKSIZE.bit.SIZE = pcksize_calc(size);
        USB->DEVICE.DeviceEndpoint[ep].EPCFG.bit.EPTYPE1 = type;
    }
    else {
        ep &= 0x7;
        USBEP[ep].DeviceDescBank[0].ADDR.reg = (uint32_t)buf;
        USBEP[ep].DeviceDescBank[0].PCKSIZE.bit.SIZE = pcksize_calc(size);
        USB->DEVICE.DeviceEndpoint[ep].EPCFG.bit.EPTYPE0 = type;
    }
}

void usb_ep_clr_out(uint8_t ep)
{
    USB->DEVICE.DeviceEndpoint[ep & 0x7].EPSTATUSCLR.reg =
        USB_DEVICE_EPSTATUS_BK0RDY;
}

void usb_ep_stall(uint8_t ep)
{
    if (ep & 0x80)
        USB->DEVICE.DeviceEndpoint[ep & 0x7].EPSTATUSSET.bit.STALLRQ1 = 1;
    else
        USB->DEVICE.DeviceEndpoint[ep & 0x7].EPSTATUSSET.bit.STALLRQ0 = 1;
}

void usb_ep_send_in(uint8_t ep, void * data, int16_t len)
{
    ep &= 0x7;
    USBEP[ep].DeviceDescBank[1].PCKSIZE.bit.BYTE_COUNT = len;
    if (data) USBEP[ep].DeviceDescBank[1].ADDR.reg = (uint32_t)data;
    USB->DEVICE.DeviceEndpoint[ep].EPINTFLAG.reg = USB_DEVICE_EPINTFLAG_TRCPT1;
    USB->DEVICE.DeviceEndpoint[ep].EPSTATUSSET.reg = USB_DEVICE_EPSTATUS_BK1RDY;
}

void usb_set_address(uint8_t addr)
{
    USB->DEVICE.DADD.reg = USB_DEVICE_DADD_DADD(addr) + USB_DEVICE_DADD_ADDEN;
}

void usb_poll(void)
{
    if (USB->DEVICE.INTFLAG.bit.EORST) {
        puts("USB: EOR\n");
        usb_on_reset();
        USB->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_EORST;
    }
    else {
        /* Handle control requests */
        if (USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.bit.RXSTP) {
            puts("USB: EP0 RXSTP\n");
            usb_on_setup_request();
            USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.reg =
                USB_DEVICE_EPINTFLAG_RXSTP;
        }
        /* Handle completed transfers events */
        for (int ep = 0; ep < USB_NUM_ENDPOINTS; ++ep) {
            if (USB->DEVICE.DeviceEndpoint[ep].EPINTFLAG.bit.TRCPT0) {
                puts("USB: EPn TRCPT0 OUT\n");
                uint8_t bc = USBEP[ep].DeviceDescBank[0].PCKSIZE.bit.BYTE_COUNT;
                usb_on_out_xfer(ep, bc);
                USB->DEVICE.DeviceEndpoint[ep].EPINTFLAG.reg = 
                    USB_DEVICE_EPINTFLAG_TRCPT0;
            }
            else if (USB->DEVICE.DeviceEndpoint[ep].EPINTFLAG.bit.TRCPT1) {
                puts("USB: EPn TRCPT1 IN\n");
                usb_on_in_xfer(ep | 0x80);
                USB->DEVICE.DeviceEndpoint[ep].EPINTFLAG.reg = 
                    USB_DEVICE_EPINTFLAG_TRCPT1;
            }
        }
    }
}
