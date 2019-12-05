#ifndef USB_H
#define USB_H

#include <stdint.h>



/* Hardware module functions (usbm.c) */
void usb_init(void);
void usb_attach(void);
void usb_poll(void);

/* Protocol related functions (usb.c) */

#endif