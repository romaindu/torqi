#ifndef TORQI_H
#define TORQI_H

#include <stdint.h>

enum {
    PHASE_A,
    PHASE_B,
    PHASES,
};

void torqi_init(void);

void torqi_calibrate(void);
void torqi_set(int8_t force);

void torqi_count(int8_t s);
int32_t torqi_sample(int32_t ph, int32_t adc);

#endif
