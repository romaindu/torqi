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

#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

#define ENCODER_RESOLUTION      4000

extern volatile int32_t motor_encoder_count;

void motor_init(void);
int  motor_powergood(void);
void motor_fault(void);
void motor_disable(void);
void motor_enable(void);

#endif
