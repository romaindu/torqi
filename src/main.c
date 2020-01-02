/*
 * main.c
 *
 * @author: Romain Durand
*/

#include "setup.h"

#include "motor.h"
#include "printf.h"
#include "reports.h"
#include "serial.h"
#include "tusb.h"

#include "torque.h"

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
    setup_ports();

    printf("\r\n\r\n=============== DEBUG ENABLED ===============\r\n");

    tusb_init();
    motor_enable();

    PORT->Group[1].DIRSET.reg = (1 << 30);

    int i = 0;
    torque_set(107);

    for (;;) {
    	//tud_task();
        //wrp.axis_x = motor_get_angle() >> 16;
        //tud_hid_report(0, &wrp, sizeof(wrp));
        if (++i > 1000) 
        {
            torque_on_encoder(1);
            i = 0;
        }
    }

    return 0;
}

void tud_mount_cb(void)
{
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

    //printf("GET REPORT ID = %d, TYPE = %d\n", report_id, report_type);

    static uint8_t bi = 0;

    if (report_type == HID_REPORT_TYPE_FEATURE) {
        if (report_id == 3 && reqlen >= sizeof(struct pid_pool_report)) {
            struct pid_pool_report * sbuf = (struct pid_pool_report *)buffer;
            sbuf->report_id = 3;
            sbuf->ram_pool_size = 1000;
            sbuf->simultaneous_effects = 8;
            sbuf->device_managed_pool = 1;
            sbuf->shared_parameter_block = 0;
            printmem(buffer, sizeof(struct pid_pool_report));
            return sizeof(struct pid_pool_report);
        }

        if (report_id == 2 && reqlen >= sizeof(struct pid_block_load_report)) {
            struct pid_block_load_report * sbuf = (struct pid_block_load_report *)buffer;
            sbuf->report_id = 2;
            sbuf->effect_block_index = ++bi;
            sbuf->block_load_status = 1;
            sbuf->ram_pool_available = 990;
            printmem(buffer, sizeof(struct pid_block_load_report));
            return sizeof(struct pid_block_load_report);
        }
    }

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

    //printf("SET REPORT ID = %d, TYPE = %d\n", report_id, report_type);
    printmem(buffer, bufsize);
}
