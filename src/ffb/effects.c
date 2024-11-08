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

#include "effects.h"

#include "util.h"

static int8_t compute_ramp(struct ffb_effect *ffbe)
{
    int32_t force, slope;

    if (!ffbe->flags.ramp || !ffbe->flags.params)
        return 0;

    if (ffbe->params.duration == 255)
        return ffbe->ramp.start;

    if (ffbe->params.duration == 0)
        return ffbe->ramp.end;

    slope = (1 << 8)*(ffbe->ramp.end - ffbe->ramp.start)/ffbe->params.duration;
    force = ffbe->ramp.start + (slope*ffbe->local_time >> 8);

    return constrain(force, -127, 127);
}

static int8_t square(uint8_t i)
{
    int8_t r;

    if (i < 128)
        r = 127;
    else
        r = -127;

    return r;
}

static int8_t sawtooth_up(uint8_t i)
{
    int32_t r = i;

    if (r != 0)
        r = r - 128;

    return r;
}

static int8_t sawtooth_down(uint8_t i)
{
    int32_t r = i;

    if (r != 0)
        r = 128 - r;

    return r;
}

static int8_t triangle(uint8_t i)
{
    int32_t r = i;

    if (r < 128)
        r = 2*r - 127;
    else
        r = 383 - 2*r;

    return r;
}

static int8_t compute_envelope(struct ffb_effect *ffbe, int8_t magnitude)
{
    uint8_t t_atck, t_fade;
    int8_t  e_atck, e_fade;
    int32_t envlp;
    int32_t slope;

    if (!ffbe->flags.params || !ffbe->flags.envelope)
        return magnitude;

    e_atck = ffbe->envelope.attack_level;
    e_fade = ffbe->envelope.fade_level;

    if (magnitude < 0) {
        e_atck = -1*e_atck;
        e_fade = -1*e_fade;
    }

    t_atck = ffbe->envelope.attack_time;

    if (ffbe->params.duration == 255)
        t_fade = 255;
    else if (ffbe->envelope.fade_time > ffbe->params.duration - t_atck)
        t_fade = t_atck;
    else
        t_fade = ffbe->params.duration - ffbe->envelope.fade_time;

    if (ffbe->local_time < t_atck) {
        slope = (magnitude - e_atck)*(1 << 8)/ffbe->envelope.attack_time;
        envlp = e_atck + (slope*ffbe->local_time >> 8);
    }
    else if (ffbe->local_time > t_fade) {
        slope = (e_fade - magnitude)*(1 << 8)/ffbe->envelope.fade_time;
        envlp = magnitude + (slope*(ffbe->local_time - t_fade) >> 8);
    }
    else {
        envlp = magnitude;
    }

    return envlp;
}

static int8_t compute_constant(struct ffb_effect *ffbe)
{
    if (!ffbe->flags.constant)
        return 0;

    return compute_envelope(ffbe, ffbe->constant.magnitude);
}

static int8_t compute_periodic(struct ffb_effect *ffbe, int8_t(*lut)(uint8_t))
{
    int32_t force;

    if (!ffbe->flags.periodic)
        return 0;

    /* Reset the phase if the effect is starting */
    if (ffbe->local_time == 0)
        ffbe->phi = 0;

    force = lut((ffbe->periodic.phase + (ffbe->phi >> 8)) & 0xFF);
    force = compute_envelope(ffbe, ffbe->periodic.magnitude)*force;
    force = rshift_round(force, 7);
    force += ffbe->periodic.offset;

    /* Phase accumulator */
    if (ffbe->periodic.period > 0)
        ffbe->phi += 0xffff/ffbe->periodic.period;

    return constrain(force, -127, 127);
}

static int8_t compute_condition(struct ffb_effect *ffbe, int32_t q)
{
    int32_t force = 0;
    int32_t lth, hth;
    int32_t lsat, hsat;

    if (!ffbe->flags.condition)
        return 0;

    lth = (ffbe->condition.cp_offset - ffbe->condition.dead_band) << 8;
    hth = (ffbe->condition.cp_offset + ffbe->condition.dead_band) << 8;

    if ((lsat = -ffbe->condition.negative_saturation) == 0)
        lsat = -127;
    if ((hsat = ffbe->condition.positive_saturation) == 0)
        hsat = 127;

    if (q < lth)
        force = ffbe->condition.negative_coefficient*(q - lth);
    else if (q > hth)
        force = ffbe->condition.positive_coefficient*(q - hth);

    force = rshift_round(force, 15);

    return constrain(force, lsat, hsat);
}

int8_t effect_compute(struct ffb_effect *ffbe, int32_t fpos, int32_t fspeed)
{
    int32_t force = 0;

    /* Check if effect is valid */
    if (!ffbe->flags.allocated || !ffbe->flags.started || !ffbe->flags.params)
        return 0;

    /* Check if effect is finished */
    if (ffbe->local_time > ffbe->params.duration)
        return 0;

    switch (ffbe->params.effect_type) {
        case CONSTANT_FORCE:
            force = compute_constant(ffbe);
            break;
        case RAMP:
            force = compute_ramp(ffbe);
            break;
        case SQUARE:
            force = compute_periodic(ffbe, square);
            break;
        case SINE:
            force = compute_periodic(ffbe, sine);
            break;
        case TRIANGLE:
            force = compute_periodic(ffbe, triangle);
            break;
        case SAWTOOTH_UP:
            force = compute_periodic(ffbe, sawtooth_up);
            break;
        case SAWTOOTH_DOWN:
            force = compute_periodic(ffbe, sawtooth_down);
            break;
        case SPRING:
            force = compute_condition(ffbe, fpos);
            break;
        case DAMPER:
            force = compute_condition(ffbe, fspeed);
            break;
        case FRICTION:
            force = compute_condition(ffbe, signed_saturate(fspeed, 16));
            break;
        default:
            break;
    }

    /* Increment time unless it is INFINITE */
    if (ffbe->local_time < 0xff)
        ffbe->local_time++;

    force = rshift_round(ffbe->params.gain * force, 8);

    return force;
}
