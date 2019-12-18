#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdint.h>

struct pi_controller {
    int32_t out;
    int32_t vec[2];
};

int32_t controller_compute(struct pi_controller *pic, int32_t err);

#endif