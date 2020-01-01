#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

void motor_init(void);
void motor_disable(void);
void motor_enable(void);
int32_t motor_angle(void);
void motor_set_angle(int32_t angle);

#endif