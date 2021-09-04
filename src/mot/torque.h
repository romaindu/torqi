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

#ifndef TORQUE_H
#define TORQUE_H

#include <stdint.h>

enum {
    PHASE_A,
    PHASE_B,
    PHASES,
};

void torque_calibrate(void);
void torque_set(int8_t force);

void torque_on_encoder_count(int8_t s);
int32_t torque_on_adc_sample(int32_t ph, int32_t adc);

#endif