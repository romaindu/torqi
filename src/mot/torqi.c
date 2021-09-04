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

#include "torqi.h"

#include "util.h"
#include "mot/motor.h"

// a-b = Kp | a+b = Ki*Ts/2
static const int16_t CONTROL_TAPS[]  = {1810, -1808};
static const uint8_t ECNT_PER_MAGREV = 80;
static const int16_t TORQUE_GAIN     = 19661;
static const int32_t PHI_GAIN        = 65536/ECNT_PER_MAGREV;

struct pi_t {
    int32_t out;
    int32_t vec[2];
};

static struct {
    struct pi_t pic[PHASES];
    uint8_t     phi;
    int8_t      force;
    int8_t      closed_loop;
} torque;

static uint8_t quadrature(uint8_t phi)
{
    uint8_t phi_q;

    phi_q = phi + ECNT_PER_MAGREV/4;

    while (phi_q > ECNT_PER_MAGREV-1)
        phi_q -= ECNT_PER_MAGREV;

    return phi_q;
}

static int32_t pi_compute(struct pi_t *pic, int32_t err)
{
    pic->vec[1] = pic->vec[0];
    pic->vec[0] = err;

    //pic->out += pic->vec[0]*CONTROL_TAPS[0];
    //pic->out += pic->vec[1]*CONTROL_TAPS[1];
    pic->out = signed_saturate(err*800, 21);

    return (pic->out << 11);
}

void torqi_init(void)
{
    torqi_set(0);
    motor_init();
}

void torqi_calibrate(void)
{
    torque.closed_loop = 0;
    torque.phi = ECNT_PER_MAGREV/4;
    torque.force = 63;
    for (uint32_t i = 0; i < 0xffffff; ++i) __asm__("nop");
    torque.force = 0;
    torque.phi = 0;
    torque.closed_loop = 1;
}

void torqi_set(int8_t force)
{
    torque.force = force;
}

void torqi_count(int8_t s)
{
    if (torque.closed_loop) {
        torque.phi += s;
        if (torque.phi == 255)
            torque.phi = ECNT_PER_MAGREV-1;
        else if (torque.phi > ECNT_PER_MAGREV-1)
            torque.phi = 0;
    }
}

int32_t torqi_sample(int32_t ph, int32_t adc)
{
    int32_t target;
    uint8_t angle;

    if (ph == PHASE_A)
        angle = torque.phi;
    else
        angle = quadrature(torque.phi);

    angle  = rshift_round(PHI_GAIN*angle, 8);
    target = rshift_round(torque.force*TORQUE_GAIN*sine(angle), 18);

    return pi_compute(&torque.pic[ph], target - adc);
}
