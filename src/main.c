/*
 * main.c
 *
 * @author: Romain Durand
*/

#include "setup.h"

#include "mot/controller.h"

volatile uint32_t adc4;

int main(void)
{
    setup_clocks();
    setup_init();
    setup_ports();

    puts("\n\n=============== DEBUG ENABLED ===============\n");

    controller_set_current(0, 2100);
    controller_set_current(0, 2048);
    motor_enable();

    static uint32_t a;

    for (;;) {
    	a = adc4;
    	putr32(a);
    }

    return 0;
}
