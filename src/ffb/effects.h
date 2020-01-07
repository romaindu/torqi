#ifndef EFFECTS_H
#define EFFECTS_H

#include <stdint.h>

#include "usb/reports.h"

enum effect_type {
    NOEFFECT,
    CONSTANT_FORCE,
    SINE,
    TRIANGLE,
    SAWTOOTH_UP,
    SAWTOOTH_DOWN,
    SPRING,
    DAMPER,
    FRICTION,
};

struct ffb_flags {
    uint8_t  allocated   :1;
    uint8_t  started     :1;
    uint8_t  params      :1;
    uint8_t  constant    :1;
    uint8_t  periodic    :1;
    uint8_t  ramp        :1;
    uint8_t  envelope    :1;
    uint8_t  condition   :1;
};

struct ffb_effect {
    struct ffb_flags         flags;
    uint8_t                  local_time;
    struct ffb_effect_params params;
    union {
        struct ffb_constant  constant;
        struct ffb_periodic  periodic;
        struct ffb_ramp      ramp;
        struct ffb_condition condition;
    };
    union {
        struct ffb_envelope  envelope;
    };
    uint32_t                 phi;
};

int8_t effect_compute(
    struct ffb_effect *ffbe,
    int16_t fpos,
    int16_t fspeed);

#endif