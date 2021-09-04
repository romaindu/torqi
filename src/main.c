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
