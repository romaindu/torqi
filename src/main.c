/*
 * main.c
 *
 * @author: Romain Durand
*/

#include "setup.h"

#include "mot/controller.h"

int main(void)
{
    setup_clocks();
    setup_init();
    setup_ports();

    puts("\n\n=============== DEBUG ENABLED ===============\n");

    motor_enable();

    for (;;) {

    }

    return 0;
}
