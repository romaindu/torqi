#ifndef TORQUE_H
#define TORQUE_H

#include <stdint.h>

enum {
    PHASE_A,
    PHASE_B,
    PHASES,
};

void torque_calibrate(void);
void torque_set(int8_t force);

void torque_on_encoder_count(int8_t s);
int32_t torque_on_adc_sample(int32_t ph, int32_t adc);

#endif