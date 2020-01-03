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

void printmem(void const *buf, uint16_t count)
{
    for (int i = 0; i < count; ++i) {
        printf("%02x ", ((char*)(buf))[i]);
    }
    printf("\n");
}

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

    PORT->Group[1].DIRSET.reg = (1 << 0) + (1 << 1) + (1 << 30);

    for (;;) {
    	tud_task();
        wrp.axis_x = motor_encoder_read();
        tud_hid_report(0, &wrp, sizeof(wrp));
    }

    return 0;
}
