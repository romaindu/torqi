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
