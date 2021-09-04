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

#include "controller.h"

#include "util.h"

static const int16_t taps[2] = {1020, -1000};

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
