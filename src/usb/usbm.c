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

static inline int _epsize_to_size_reg(int s)
{
    if (s <= 8)     return 0;
    if (s <= 16)    return 1;
    if (s <= 32)    return 2;
    else /*64*/     return 3;
}

void usb_configure_ep0(void)
{
    /* Assign buffer addresses and size */
    USBEP[0].DeviceDescBank[0].ADDR.reg = (uint32_t)&epZeroOutBuf;
    USBEP[0].DeviceDescBank[0].PCKSIZE.reg =
        USB_HOST_PCKSIZE_SIZE(_epsize_to_size_reg(USB_EP0_SIZE));
    USBEP[0].DeviceDescBank[1].ADDR.reg = (uint32_t)&epZeroInBuf;
    USBEP[0].DeviceDescBank[1].PCKSIZE.reg =
        USB_HOST_PCKSIZE_SIZE(_epsize_to_size_reg(USB_EP0_SIZE));

    /* Set EP0 as a control (IN/OUT) endpoint */
    USB->DEVICE.DeviceEndpoint[0].EPCFG.reg =   USB_DEVICE_EPCFG_EPTYPE0(1) +
                                                USB_DEVICE_EPCFG_EPTYPE1(1);
}

void usb_poll(void)
{
    if (USB->DEVICE.INTFLAG.bit.EORST) {
        puts("USB: End Of Reset\n");
        usb_configure_ep0();
        USB->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_EORST;
    }
    else {
        /* Handle control requests */
        if (USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.bit.RXSTP) {
            puts("USB: EP0 RXSTP interrupt\n");
            memcpy(&setupPacket, epZeroOutBuf, sizeof(SetupPacket_t));
            putm(&epZeroOutBuf, sizeof(SetupPacket_t));
            USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.reg = 
                USB_DEVICE_EPINTFLAG_RXSTP;
        }
        else if (USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.bit.TRCPT0) {
            puts("USB: EP0 TRCPT IN interrupt\n");
            USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.reg = 
                USB_DEVICE_EPINTFLAG_TRCPT0;
        }
        else if (USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.bit.TRCPT1) {
            puts("USB: EP0 TRCPT OUT interrupt\n");
            USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.reg = 
                USB_DEVICE_EPINTFLAG_TRCPT1;
        }

        /* Handle transfers on other endpoints */
        for (int i = 1; i < USB_NUM_ENDPOINTS; ++i) {
            if (USB->DEVICE.DeviceEndpoint[i].EPINTFLAG.bit.TRCPT0) {
                puts("USB: EPn TRCPT0 interrupt\n");
                USB->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg = 
                    USB_DEVICE_EPINTFLAG_TRCPT0;
            }
            else if (USB->DEVICE.DeviceEndpoint[i].EPINTFLAG.bit.TRCPT1) {
                puts("USB: EPn TRCPT1 interrupt\n");
                USB->DEVICE.DeviceEndpoint[i].EPINTFLAG.reg = 
                    USB_DEVICE_EPINTFLAG_TRCPT1;
            }
        }
    }
}
