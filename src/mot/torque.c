/*
 * torque.c
 *
 * @author: Romain Durand
*/

#include "torque.h"

#include "printf.h"
#include "controller.h"

#define ECNT_PER_MAGREV     80

static struct {
    struct pi_controller    pic[PHASES];
    int16_t                 flux[PHASES];
    uint8_t                 phi;
    int8_t                  force;
    int8_t                  closed_loop;
} torque;

static int16_t sine[ECNT_PER_MAGREV] = 
            { 0x0000, 0x0808, 0x1004, 0x17e7, 0x1fa4, 0x272f, 0x2e7c, 0x3580,
              0x3c2f, 0x4280, 0x4867, 0x4ddc, 0x52d7, 0x574e, 0x5b3c, 0x5e9a,
              0x6162, 0x6391, 0x6522, 0x6614, 0x6665, 0x6614, 0x6522, 0x6391,
              0x6162, 0x5e9a, 0x5b3c, 0x574e, 0x52d7, 0x4ddc, 0x4867, 0x4280,
              0x3c2f, 0x3580, 0x2e7c, 0x272f, 0x1fa4, 0x17e7, 0x1004, 0x0808,
              0x0000, 0xf7f8, 0xeffc, 0xe819, 0xe05c, 0xd8d1, 0xd184, 0xca80,
              0xc3d1, 0xbd80, 0xb799, 0xb224, 0xad29, 0xa8b2, 0xa4c4, 0xa166,
              0x9e9e, 0x9c6f, 0x9ade, 0x99ec, 0x999b, 0x99ec, 0x9ade, 0x9c6f,
              0x9e9e, 0xa166, 0xa4c4, 0xa8b2, 0xad29, 0xb224, 0xb799, 0xbd80,
              0xc3d1, 0xca80, 0xd184, 0xd8d1, 0xe05c, 0xe819, 0xeffc, 0xf7f8 };

static uint8_t quadrature(uint8_t phi)
{
    uint8_t phi_q;

    phi_q = phi + ECNT_PER_MAGREV/4;

    while (phi_q > ECNT_PER_MAGREV-1)
        phi_q -= ECNT_PER_MAGREV;

    return phi_q;
}

void torque_calibrate(void)
{
    torque.flux[PHASE_A] = sine[20];
    torque.flux[PHASE_B] = sine[quadrature(20)];
    torque.force = 63;
    for (uint32_t i = 0; i < 0xffffff; ++i) __asm__("nop");
    torque.force = 0;
    torque.phi = 0;
    torque.closed_loop = 1;
}

void torque_set(int8_t force)
{
    torque.force = force;
}

void torque_on_encoder_count(int8_t s)
{
    torque.phi += s;

    if (torque.phi == 255)
        torque.phi = ECNT_PER_MAGREV-1;
    else if (torque.phi > ECNT_PER_MAGREV-1)
        torque.phi = 0;

    if (torque.closed_loop) {
        torque.flux[PHASE_A] = sine[torque.phi];
        torque.flux[PHASE_B] = sine[quadrature(torque.phi)];
    }
}

int32_t torque_on_adc_sample(int32_t ph, int32_t adc)
{
    int32_t target;

    target = torque.force*torque.flux[ph] >> 11;

    return controller_compute(&torque.pic[ph], target - adc + 2048);
}
