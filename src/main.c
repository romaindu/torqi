/*
 * main.c
 *
 * @author: Romain Durand
*/

#include "samd21.h"

#include "com/serial.h"
#include "usb/usb.h"

int main(void)
{
    com_init();
    usbm_init();
    
    usbm_attach();

    puts("\n\n======== Restarting ========\n");

    for (;;) {
        usbm_poll();
    }

    return 0;
}

void usb_set_configuration(uint16_t numConfig)
{
    static char ep1in[16], ep1out[16];
    usbm_configure_ep(0x01, 16, EP_INTERRUPT, ep1out);
    usbm_configure_ep(0x81, 16, EP_INTERRUPT, ep1in);
}
