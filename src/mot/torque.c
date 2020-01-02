/*
 * torque.c
 *
 * @author: Romain Durand
*/

#include "torque.h"

#include "controller.h"

#define ECNT_PER_MAGREV     80

static struct {
    struct pi_controller    pic[PHASES];
    volatile int16_t        tgt[PHASES];
    volatile int8_t         phi[PHASES];
    volatile int8_t         trq;
    int16_t                 sine[ECNT_PER_MAGREV];
} g_torque = {
    .pic = {0},
    .tgt = {0},
    .phi = {0, ECNT_PER_MAGREV/4},
    .trq = 0,
    .sine = { 0x0000, 0x0808, 0x1004, 0x17e7, 0x1fa4, 0x272f, 0x2e7c, 0x3580,
              0x3c2f, 0x4280, 0x4867, 0x4ddc, 0x52d7, 0x574e, 0x5b3c, 0x5e9a,
              0x6162, 0x6391, 0x6522, 0x6614, 0x6665, 0x6614, 0x6522, 0x6391,
              0x6162, 0x5e9a, 0x5b3c, 0x574e, 0x52d7, 0x4ddc, 0x4867, 0x4280,
              0x3c2f, 0x3580, 0x2e7c, 0x272f, 0x1fa4, 0x17e7, 0x1004, 0x0808,
              0x0000, 0xf7f8, 0xeffc, 0xe819, 0xe05c, 0xd8d1, 0xd184, 0xca80,
              0xc3d1, 0xbd80, 0xb799, 0xb224, 0xad29, 0xa8b2, 0xa4c4, 0xa166,
              0x9e9e, 0x9c6f, 0x9ade, 0x99ec, 0x999b, 0x99ec, 0x9ade, 0x9c6f,
              0x9e9e, 0xa166, 0xa4c4, 0xa8b2, 0xad29, 0xb224, 0xb799, 0xbd80,
              0xc3d1, 0xca80, 0xd184, 0xd8d1, 0xe05c, 0xe819, 0xeffc, 0xf7f8 },
};

static inline void compute_current_targets(void)
{
    int32_t a;

    for (int ph = 0; ph < PHASES; ++ph) {
        a = g_torque.trq*g_torque.sine[g_torque.phi[ph]];
        g_torque.tgt[ph] = 2048 + (a >> 11);
    }
}

void torque_set(int8_t trq)
{
    g_torque.trq = trq;

    return compute_current_targets();
}

void torque_on_encoder(int8_t s)
{
    int8_t a;

    for (int ph = 0; ph < PHASES; ++ph) {
        a = g_torque.phi[ph] + s;
        if (a > ECNT_PER_MAGREV-1)
            a = 0;
        else if (a < 0)
            a = ECNT_PER_MAGREV-1;
        g_torque.phi[ph] = a;
    }

    return compute_current_targets();
}

int32_t torque_on_adc_sample(int32_t ph, int32_t adc)
{
    return controller_compute(&g_torque.pic[ph], g_torque.tgt[ph] - adc);
}
