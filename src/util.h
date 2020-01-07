#ifndef UTIL_H
#define UTIL_H

static inline int32_t constrain(int32_t val, int32_t min, int32_t max)
{
    int32_t r;

    if (val > max)
        r = max;
    else if (val < min)
        r = min;
    else
        r = val;

    return r;
}

static inline int32_t signed_saturate(int32_t val, int32_t sat)
{
    int32_t min;
    int32_t max;

    if ((sat >= 1) && (sat <= 32)) {
        max = (1 << (sat - 1)) - 1;
        min = -max - 1;

        if (val > max)
            return max;
        if (val < min)
            return min;
    }

    return val;
}

#endif