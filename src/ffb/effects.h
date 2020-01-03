#ifndef EFFECTS_H
#define EFFECTS_H

#include <stdint.h>

#include "usb/reports.h"

enum effect_type {
    NOEFFECT            = 0,
    CONSTANT_FORCE      = 1,
    RAMP                = 2,
    SQUARE              = 3,
    SINE                = 4,
    TRIANGLE            = 5,
    SAWTOOTH_UP         = 6,
    SAWTOOTH_DOWN       = 7,
    SPRING              = 8,
    DAMPER              = 9,
    INERTIA             = 10,
    FRICTION            = 11,
};

struct ffb_loaded_flags {
    uint8_t  effect      :1;
    uint8_t  params      :1;
    uint8_t  constant    :1;
    uint8_t  periodic    :1;
    uint8_t  ramp        :1;
    uint8_t  envelope    :1;
    uint8_t  condition_0 :1;
    uint8_t  condition_1 :1;
};

struct ffb_effect {
    struct ffb_loaded_flags  loaded;
    uint8_t                  local_time;
    uint16_t                 phase;
    struct ffb_effect_params params;
    union {
        struct ffb_constant  constant;
        struct ffb_periodic  periodic;
        struct ffb_ramp      ramp;
        struct ffb_condition condition_0;
    };
    union {
        struct ffb_envelope  envelope;
        struct ffb_condition condition_1;
    };
};

void effect_reset(struct ffb_effect * ffbe);

#endif