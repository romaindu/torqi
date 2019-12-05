/*
 * main.c
 *
 * @author: Romain Durand
*/

#include "samd21.h"

#include "com/serial.h"
#include "setup.h"
#include "usb/usb.h"

int main(void)
{
    setup_clocks();
    setup_ports();

    com_init();
    usb_init();
    
    usb_attach();

    puts("\n======== Restarting ========\n");
    putm((int*)0x20000000, 16);

    for (;;) {
        usb_poll();
    }

    return 0;
}
