/*
 * wheel.c
 *
 * @author: Romain Durand
*/

#include "wheel.h"

#include "tusb.h"

#include "util.h"
#include "mot/motor.h"
#include "usb/reports.h"

static const int8_t  ENDSTOP_GAIN = 50;
static const int32_t DEGREES_PER_COUNT = -65536*360/ENCODER_RESOLUTION;
volatile int16_t steering_range = 900;

void wheel_task(void)
{
    int32_t angle;

    if (tud_hid_ready()) {
        static struct wheel_report usb_whl_report = {
            .report_id = WHEEL_REPORT_ID,
        };

        angle = DEGREES_PER_COUNT*motor_encoder_count;
        usb_whl_report.axis_x = signed_saturate(angle/steering_range, 16);

        tud_hid_report(0, &usb_whl_report, sizeof(usb_whl_report));
    }
}

int32_t wheel_endstop(int32_t force)
{
    int32_t f, angle, lim;

    angle = DEGREES_PER_COUNT*motor_encoder_count;
    lim = (steering_range << 15);

    if (angle > lim) {
        f = lim - angle;
        force = signed_saturate(ENDSTOP_GAIN*f >> 16, 8);
    }
    else if (angle < -lim) {
        f = -angle - lim;
        force = signed_saturate(ENDSTOP_GAIN*f >> 16, 8);
    }

    return force;
}
