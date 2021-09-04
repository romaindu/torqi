/*
 *	Copyright 2020 Romain Durand
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
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
