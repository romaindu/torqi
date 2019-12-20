/*
 * controller.c
 *
 * @author: Romain Durand
*/

#include "controller.h"

static const int16_t taps[2] = {5122, -5112};

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

int32_t controller_compute(struct pi_controller *pic, int32_t err)
{
	err = signed_saturate(err, 15);

	pic->vec[1] = pic->vec[0];
	pic->vec[0] = err;

	pic->out += pic->vec[0]*taps[0];
	pic->out += pic->vec[1]*taps[1];

	pic->out = signed_saturate(pic->out, 21);

	return (pic->out << 11);
}
