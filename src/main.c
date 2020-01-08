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
#include "usb/reports.h"
#include "com/serial.h"

static struct wheel_report wrp = {.report_id = 1};

int main(void)
{
    setup_clocks();

    serial_init();
    motor_init();
    ffb_init();

    setup_ports();

    tusb_init();

    motor_enable();
    ffb_reset();

    printf("\n\n====== RESET ======\n");

    PORT->Group[1].DIRSET.reg = (1 << 0) + (1 << 1) + (1 << 30);

    for (;;) {
    	tud_task();
        wrp.axis_x = motor_encoder_read() << 3;
        tud_hid_report(0, &wrp, sizeof(wrp));
    }

    return 0;
}
