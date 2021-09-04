#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

int8_t  sine(uint8_t i);
int32_t rshift_round(int32_t val, int32_t rsh);
int32_t constrain(int32_t val, int32_t min, int32_t max);
int32_t signed_saturate(int32_t val, int32_t sat);

#endif
