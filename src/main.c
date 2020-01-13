/*
 * main.c
 *
 * @author: Romain Durand
*/

#include "setup.h"

#include "printf.h"
#include "tusb.h"

#include "ffb/ffb.h"
#include "mot/motor.h"
#include "mot/torque.h"
#include "usb/reports.h"
#include "com/serial.h"
#include "whl/wheel.h"

int main(void)
{
    setup_clocks();

    serial_init();
    motor_init();

    setup_ports();

    printf("\n\n====== RESET ======\n");

    tusb_init();

    motor_enable();
    torque_calibrate();

    ffb_init();

    for(;;) {
        tud_task();
        wheel_task();
    }

    return 0;
}
