#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

void motor_init(void);
void motor_disable(void);
void motor_enable(void);

int32_t motor_encoder_read(void);
void motor_encoder_write(int32_t val);

#endif