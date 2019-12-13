/*
 * main.c
 *
 * @author: Romain Durand
*/

#include "setup.h"

void mot_pwm(uint32_t pwm);

int main(void)
{
    setup_clocks();
    setup_init();
    setup_ports();

    puts("\n\n=============== DEBUG ENABLED ===============\n");

    PORT->Group[1].DIRSET.reg = (1 << 17) + (1 << 30);
    mot_enable();

    for (;;) {
        //mot_enable();
        //for (int i = 0; i < 0xffffff; ++i) { __asm__("nop"); }
        //mot_disable();
        //for (int i = 0; i < 0xffffff; ++i) { __asm__("nop"); }
    }

    return 0;
}
