/*
 * main.c
 *
 * @author: Romain Durand
*/

#include "setup.h"

#include "motor.h"
#include "printf.h"
#include "serial.h"
#include "tusb.h"

int main(void)
{
    setup_clocks();
    serial_init();
    motor_init();
    setup_ports();

    printf("\r\n\r\n=============== DEBUG ENABLED ===============\r\n");

    tusb_init();
    //motor_enable();

    for (;;) {
    	tud_task();
    }

    return 0;
}

uint16_t tud_hid_get_report_cb(
	uint8_t report_id,
	hid_report_type_t report_type,
	uint8_t* buffer,
	uint16_t reqlen)
{
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

void tud_hid_set_report_cb(
	uint8_t report_id,
	hid_report_type_t report_type, 
	uint8_t const* buffer,
	uint16_t bufsize)
{
  (void) report_id;
  (void) report_type;
}
