#ifndef TORQUE_H
#define TORQUE_H

#include <stdint.h>

enum {
    PHASE_A,
    PHASE_B,
};

int32_t torque_on_adc_sample(int32_t ph, int32_t adc);

#endif