/*
 * controller.c
 *
 * @author: Romain Durand
*/

#include "controller.h"

#define COEF0 	5122
#define COEF1 	-5112

struct controller_phase {
	int32_t set;
	int32_t out;
	int32_t vec[2];
} phase_a, phase_b;

static inline int32_t signed_saturate(int32_t val, int32_t sat)
{
	int32_t min;
	int32_t max;

	if ((sat >= 1) && (sat <= 32)) {
		max = (1 << (sat - 1)) - 1;
		min = -max - 1;

		if (val > max)
			return max;
		if (val < min)
			return min;
	}

	return val;
}

int32_t controller_compute_pwm(int32_t ph, int32_t adc)
{
	struct controller_phase *cp;

	if (ph == PHASE_A)
		cp = &phase_a;
	else
		cp = &phase_b;

	cp->vec[1] = cp->vec[0];
	cp->vec[0] = cp->set - adc;

	cp->out += cp->vec[0]*COEF0;
	cp->out += cp->vec[1]*COEF1;

	cp->out = signed_saturate(cp->out, 31);

	return signed_saturate(cp->out, 21) << 11;
}

void controller_set_current(int32_t ph, int32_t tg)
{
	if (ph == PHASE_A)
		phase_a.set = tg;
	else
		phase_b.set = tg;
}
