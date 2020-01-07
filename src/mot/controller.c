/*
 * controller.c
 *
 * @author: Romain Durand
*/

#include "controller.h"

#include "util.h"

static const int16_t taps[2] = {5122, -5112};

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
