#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdint.h>

enum {
	PHASE_A,
	PHASE_B,
};

int32_t controller_compute_pwm(int32_t ph, int32_t adc);
void controller_set_current(int32_t ph, int32_t tg);

#endif