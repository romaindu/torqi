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
