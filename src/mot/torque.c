/*
 * torque.c
 *
 * @author: Romain Durand
*/

#include "torque.h"

#include "util.h"

#include "controller.h"

static const int8_t  ECNT_PER_MAGREV = 80;
static const int8_t  TORQUE_GAIN     = 105;
static const int32_t PHI_GAIN        = 65536/ECNT_PER_MAGREV;

static struct {
    struct pi_controller    pic[PHASES];
    uint8_t                 phi;
    int8_t                  force;
    int8_t                  closed_loop;
} torque;

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
    torque.closed_loop = 0;
    torque.phi = 20;
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
    if (torque.closed_loop) {
        torque.phi += s;
        if (torque.phi == 255)
            torque.phi = ECNT_PER_MAGREV-1;
        else if (torque.phi > ECNT_PER_MAGREV-1)
            torque.phi = 0;
    }
}

int32_t torque_on_adc_sample(int32_t ph, int32_t adc)
{
    int32_t target;
    uint8_t angle;

    if (ph == PHASE_A)
        angle = torque.phi;
    else
        angle = quadrature(torque.phi);

    angle  = rshift_round(PHI_GAIN*angle, 8);
    target = rshift_round(torque.force*TORQUE_GAIN*sine(angle), 10);

    return controller_compute(&torque.pic[ph], target - adc + 2048);
}
