/*
 * motor.c
 *
 * @author: Romain Durand
*/

#include "wheel.h"

#include "tusb.h"

#include "util.h"
#include "mot/motor.h"
#include "usb/reports.h"

static const int8_t  ENDSTOP_GAIN = 50;
static const int32_t DEGREES_PER_COUNT = 65536*360/ENCODER_RESOLUTION;

static int16_t steering_range = 900;

static inline int32_t get_wheel_angle_q16(void)
{
    return DEGREES_PER_COUNT*motor_encoder_read();
}

int8_t wheel_endstop_force(void)
{
    int32_t force, angle, lim;

    angle = get_wheel_angle_q16();
    lim = (steering_range << 15);

    if (angle > lim)
        force = angle - lim;
    else if (angle < -lim)
        force = angle + lim;
    else
        force = 0;

    force = signed_saturate(ENDSTOP_GAIN*force >> 16, 8);

    return force;
}

void wheel_task(void)
{
    int32_t angle;

    static struct wheel_report usb_whl_report = {
        .report_id = WHEEL_REPORT_ID,
        0
    };

    angle = get_wheel_angle_q16();
    usb_whl_report.axis_x = signed_saturate(angle/steering_range, 16);

    if (tud_hid_ready()) {
        tud_hid_report(0, &usb_whl_report, sizeof(usb_whl_report));
    }
}
