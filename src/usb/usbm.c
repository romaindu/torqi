/*
 * usbm.c
 *
 * @author: Romain Durand
*/

#include "usb.h"

#include "samd21.h"

UsbDeviceDescriptor USBEP[8];

void usb_init(void)
{
    USB->DEVICE.CTRLA.bit.SWRST = 1;

    while (USB->DEVICE.SYNCBUSY.bit.SWRST);

    USB->DEVICE.CTRLA.reg = USB_CTRLA_MODE_DEVICE + USB_CTRLA_ENABLE;

    while (USB->DEVICE.SYNCBUSY.bit.ENABLE);
}

void usb_attach(void)
{
    USB->DEVICE.CTRLB.bit.DETACH = 0;
}

void usb_poll(void)
{

}
