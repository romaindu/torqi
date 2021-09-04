/*
 * main.c
 *
 * @author: Romain Durand
*/

#include "setup.h"

#include "tusb.h"
#include "mot/torqi.h"
#include "mot/motor.h"
#include "ffb/ffb.h"
#include "whl/wheel.h"

int main(void)
{
    setup_clocks();
    setup_ports();

    torqi_init();

    while (!motor_powergood());

    tusb_init();

    motor_enable();
    torqi_calibrate();

    ffb_init();

    for(;;) {
        tud_task();
        wheel_task();
    }

    return 0;
}

void USB_Handler(void)
{
    tud_int_handler(0);
}
