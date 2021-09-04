#ifndef WHEEL_H
#define WHEEL_H

#include <stdint.h>

extern volatile int16_t steering_range;

void wheel_task(void);
int32_t wheel_endstop(int32_t force);

#endif
