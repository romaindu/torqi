/*
 * main.c
 *
 * @author: Romain Durand
*/

#include "setup.h"

int main(void)
{
    setup_clocks();
    setup_init();
    setup_ports();

    puts("\n\n======== Restarting ========\n");
    PORT->Group[1].DIRSET.reg = (1 << 30);

    for (;;) {
        mot_enable();
        PORT->Group[1].OUTCLR.reg = (1 << 30);
        for (int i = 0; i < 0xffffff; ++i) { __asm__("nop"); }
        mot_disable();
        PORT->Group[1].OUTSET.reg = (1 << 30);
        for (int i = 0; i < 0xffffff; ++i) { __asm__("nop"); }
    }

    return 0;
}
