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
    usb_init();
    
    usb_attach();

    puts("\n======== Restarting ========\n");

    for (;;) {
        usb_poll();
    }

    return 0;
}
