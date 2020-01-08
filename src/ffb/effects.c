/*
 * effects.c
 *
 * @author: Romain Durand
*/

#include "effects.h"

#include "util.h"

#define FRICTION_FRACTION_OF_MAXSPEED   10

static int8_t compute_constant(struct ffb_effect *ffbe)
{
    if (!ffbe->flags.constant)
        return 0;

    return ffbe->constant.magnitude;
}

static int8_t compute_ramp(struct ffb_effect *ffbe)
{
    int32_t force, slope;

    if (!ffbe->flags.ramp)
        return 0;

    slope = (1 << 16)*(ffbe->ramp.end - ffbe->ramp.start)/ffbe->params.duration;
    force = ffbe->ramp.start + (slope*ffbe->local_time >> 16);

    return constrain(force, -127, 127);
}

static int8_t sine(uint8_t i)
{
    static int8_t lut[256] = {0, 3, 6, 9, 12, 16, 19, 22, 25, 28, 31, 34, 37, 40, 43, 46, 49, 51, 54, 57, 60, 63, 65, 68, 71, 73, 76, 78, 81, 83, 85, 88, 90, 92, 94, 96, 98, 100, 102, 104, 106, 107, 109, 110, 112, 113, 115, 116, 117, 118, 120, 121, 122, 122, 123, 124, 125, 125, 126, 126, 126, 127, 127, 127, 127, 127, 127, 127, 126, 126, 126, 125, 125, 124, 123, 122, 122, 121, 120, 118, 117, 116, 115, 113, 112, 111, 109, 107, 106, 104, 102, 100, 98, 96, 94, 92, 90, 88, 85, 83, 81, 78, 76, 73, 71, 68, 65, 63, 60, 57, 54, 51, 49, 46, 43, 40, 37, 34, 31, 28, 25, 22, 19, 16, 12, 9, 6, 3, 0, -3, -6, -9, -12, -16, -19, -22, -25, -28, -31, -34, -37, -40, -43, -46, -49, -51, -54, -57, -60, -63, -65, -68, -71, -73, -76, -78, -81, -83, -85, -88, -90, -92, -94, -96, -98, -100, -102, -104, -106, -107, -109, -110, -112, -113, -115, -116, -117, -118, -120, -121, -122, -122, -123, -124, -125, -125, -126, -126, -126, -127, -127, -127, -127, -127, -127, -127, -126, -126, -126, -125, -125, -124, -123, -122, -122, -121, -120, -118, -117, -116, -115, -113, -112, -111, -109, -107, -106, -104, -102, -100, -98, -96, -94, -92, -90, -88, -85, -83, -81, -78, -76, -73, -71, -68, -65, -63, -60, -57, -54, -51, -49, -46, -43, -40, -37, -34, -31, -28, -25, -22, -19, -16, -12, -9, -6, -3};

    return lut[i];
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

static int8_t compute_periodic(struct ffb_effect *ffbe, int8_t(*lut)(uint8_t))
{
    int32_t force;

    if (!ffbe->flags.periodic)
        return 0;

    /* Reset the phase if the effect is starting */
    if (ffbe->local_time == 0)
        ffbe->phi = 0;

    force = lut((ffbe->periodic.phase + (ffbe->phi >> 16)) & 0xFF);
    force = (ffbe->periodic.magnitude*force >> 7) + ffbe->periodic.offset;

    /* Phase accumulator */
    ffbe->phi += (1 << 24)/ffbe->periodic.period;

    return constrain(force, -127, 127);
}

static int16_t friction_q(int16_t fspeed)
{
    return signed_saturate((fspeed >> 7)*FRICTION_FRACTION_OF_MAXSPEED, 9);
}

static int8_t compute_condition(struct ffb_effect *ffbe, int16_t q)
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
        force = ffbe->condition.negative_coefficient*(q - lth) >> 8;
    else if (q > hth)
        force = ffbe->condition.positive_coefficient*(q - hth) >> 8;

    return constrain(force, lsat, hsat);
}

int8_t effect_compute(
    struct ffb_effect *ffbe,
    int16_t fpos,
    int16_t fspeed)
{
    int8_t force = 0;

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
            force = compute_condition(ffbe, friction_q(fspeed));
            break;
        default:
            break;
    }

    /* Increment time unless it is INFINITE */
    if (ffbe->local_time < 0xff)
        ffbe->local_time++;

    force = (ffbe->params.gain * force) >> 8;

    return force;
}
