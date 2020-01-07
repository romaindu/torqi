/*
 * ffb.c
 *
 * @author: Romain Durand
*/

#include "ffb.h"

#include <string.h>

#include "sam.h"
#include "printf.h"
#include "util.h"

#include "mot/motor.h"
#include "ffb/effects.h"
#include "usb/reports.h"

#define FFB_POSITION_COEF       5
#define FFB_SPEED_COEF          500

static struct ffb_effect pid_effects_pool[FFB_MAX_EFFECTS] = {0};

static struct pid_block_load_report block_load_report = {
    .report_id = BLOCK_LOAD_REPORT_ID,
    .effect_block_index = 0,
    .block_load_status = 0,
    .ram_pool_available = sizeof(pid_effects_pool)
};

static const struct pid_pool_report pool_report = {
    .report_id = PID_POOL_REPORT_ID,
    .ram_pool_size = sizeof(pid_effects_pool),
    .simultaneous_effects = FFB_MAX_EFFECTS,
    .device_managed_pool = 1,
    .shared_parameter_block = 0,
};

static uint8_t ffb_gain = 0;

static inline void __FFB_ENTER_CRITICAL(void)
{
    NVIC_DisableIRQ(TC3_IRQn);
}

static inline void __FFB_LEAVE_CRITICAL(void)
{
    NVIC_EnableIRQ(TC3_IRQn);
}

void ffb_init(void)
{
    TC3->COUNT16.CTRLA.bit.SWRST = 1;
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY);

    TC3->COUNT16.INTENSET.reg = TC_INTENSET_OVF;
    TC3->COUNT16.CC[0].reg = 47999;
    TC3->COUNT16.CTRLA.reg = TC_CTRLA_WAVEGEN_MFRQ;
    TC3->COUNT16.CTRLA.bit.ENABLE = 1;

    DAC->CTRLA.bit.ENABLE = 1;
    while (DAC->STATUS.bit.SYNCBUSY);

    DAC->CTRLB.reg = DAC_CTRLB_REFSEL_AVCC + DAC_CTRLB_EOEN;
    DAC->CTRLA.bit.ENABLE = 1;

    NVIC_SetPriority(TC3_IRQn, 3);
    NVIC_EnableIRQ(TC3_IRQn);
}

void ffb_reset(void)
{
    memset(&pid_effects_pool, 0, sizeof(pid_effects_pool));
}

void ffb_on_set_effect_report(uint8_t const *report)
{
    struct set_effect_report *r = (struct set_effect_report *)report;
    uint8_t idx = r->effect_block_index - 1;

    if (idx < FFB_MAX_EFFECTS) {
        __FFB_ENTER_CRITICAL();
        memcpy(&pid_effects_pool[idx].params,
               r->payload,
               sizeof(struct ffb_effect_params));
        pid_effects_pool[idx].flags.params = 1;
        __FFB_LEAVE_CRITICAL();
    }
}

void ffb_on_set_envelope_report(uint8_t const *report)
{
    struct set_envelope_report *r = (struct set_envelope_report *)report;
    uint8_t idx = r->effect_block_index - 1;

    if (idx < FFB_MAX_EFFECTS) {
        __FFB_ENTER_CRITICAL();
        memcpy(&pid_effects_pool[idx].envelope,
               r->payload,
               sizeof(struct ffb_envelope));
        pid_effects_pool[idx].flags.envelope = 1;
        __FFB_LEAVE_CRITICAL();
    }
}

void ffb_on_set_condition_report(uint8_t const *report)
{
    struct set_condition_report *r = (struct set_condition_report *)report;
    uint8_t idx = r->effect_block_index - 1;

    /* Only accept 0 (first axis) */
    if (r->parameter_block_offset)
        return;

    if (idx < FFB_MAX_EFFECTS) {
        __FFB_ENTER_CRITICAL();
        memcpy(&pid_effects_pool[idx].condition,
               r->payload,
               sizeof(struct ffb_condition));
        pid_effects_pool[idx].flags.condition = 1;
        __FFB_LEAVE_CRITICAL();
    }
}

void ffb_on_set_periodic_report(uint8_t const *report)
{
    struct set_periodic_report *r = (struct set_periodic_report *)report;
    uint8_t idx = r->effect_block_index - 1;

    if (idx < FFB_MAX_EFFECTS) {
        __FFB_ENTER_CRITICAL();
        memcpy(&pid_effects_pool[idx].periodic,
               r->payload,
               sizeof(struct ffb_periodic));
        pid_effects_pool[idx].flags.periodic = 1;
        __FFB_LEAVE_CRITICAL();
    }
}

void ffb_on_set_constant_force_report(uint8_t const *report)
{
    struct set_constant_force_report *r = (struct set_constant_force_report *)report;
    uint8_t idx = r->effect_block_index - 1;

    if (idx < FFB_MAX_EFFECTS) {
        __FFB_ENTER_CRITICAL();
        memcpy(&pid_effects_pool[idx].constant,
               r->payload,
               sizeof(struct ffb_constant));
        pid_effects_pool[idx].flags.constant = 1;
        __FFB_LEAVE_CRITICAL();
    }
}

void ffb_on_set_ramp_force_report(uint8_t const *report)
{
    struct set_ramp_force_report *r = (struct set_ramp_force_report *)report;
    uint8_t idx = r->effect_block_index - 1;

    if (idx < FFB_MAX_EFFECTS) {
        __FFB_ENTER_CRITICAL();
        memcpy(&pid_effects_pool[idx].ramp,
               r->payload,
               sizeof(struct ffb_ramp));
        pid_effects_pool[idx].flags.ramp = 1;
        __FFB_LEAVE_CRITICAL();
    }
}

void ffb_on_create_new_effect_report(uint8_t const *report)
{
    uint8_t i;

    for (i = 0; i < FFB_MAX_EFFECTS; ++i) {
        if (!pid_effects_pool[i].flags.allocated) {
            __FFB_ENTER_CRITICAL();
            memset(&pid_effects_pool[i], 0, sizeof(struct ffb_effect));
            pid_effects_pool[i].flags.allocated = 1;
            __FFB_LEAVE_CRITICAL();
            block_load_report.effect_block_index = i + 1;
            block_load_report.block_load_status = BLOCK_LOAD_SUCCESS;
            return;
        }
    }
    block_load_report.effect_block_index = 0;
    block_load_report.block_load_status = BLOCK_LOAD_FULL;
}

void ffb_on_effect_operation_report(uint8_t const *report)
{
    struct effect_operation_report *r = (struct effect_operation_report *)report;
    uint8_t idx = r->effect_block_index - 1;
    uint8_t op = r->effect_operation;
    uint8_t i;

    if (idx < FFB_MAX_EFFECTS) {
        __FFB_ENTER_CRITICAL();
        if (op == EFFECT_START) {
            pid_effects_pool[idx].flags.started = 1;
            pid_effects_pool[idx].local_time = 0;
        } else if (op == EFFECT_START_SOLO) {
            for (i = 0; i < FFB_MAX_EFFECTS; ++i) {
                pid_effects_pool[idx].local_time = 0;
                if (i != idx)
                    pid_effects_pool[i].flags.started = 0;
                else
                    pid_effects_pool[i].flags.started = 1;
            }
        } else {
            pid_effects_pool[idx].flags.started = 0;
        }
        __FFB_LEAVE_CRITICAL();
    }
}

void ffb_on_pid_block_free_report(uint8_t const *report)
{
    struct pid_block_free_report *r = (struct pid_block_free_report *)report;
    uint8_t idx = r->effect_block_index - 1;

    if (idx < FFB_MAX_EFFECTS) {
        pid_effects_pool[idx].flags.allocated = 0;
    }
}

void ffb_on_device_control_report(uint8_t const *report)
{
    struct pid_device_control_report *r = (struct pid_device_control_report *)report;
    uint8_t i;

    switch (r->pid_device_control) {
        case ENABLE_ACTUATORS:
            motor_enable();
            break;
        case DISABLE_ACTUATORS:
            motor_disable();
            break;
        case STOP_ALL_EFFECTS:
            __FFB_ENTER_CRITICAL();
            for (i = 0; i < FFB_MAX_EFFECTS; ++i) {
                pid_effects_pool[i].flags.started = 0;
            }
            __FFB_LEAVE_CRITICAL();
            break;
        case DEVICE_RESET:
            ffb_reset();
            break;
        case DEVICE_PAUSE:
            break;
        case DEVICE_CONTINUE:
            break;
    }
}

void ffb_on_device_gain_report(uint8_t const *report)
{
    struct device_gain_report *r = (struct device_gain_report *)report;

    ffb_gain = r->gain;
}

int ffb_on_get_pid_pool_report(uint8_t *report)
{
    memcpy(report, &pool_report, sizeof(pool_report));
    return sizeof(struct pid_pool_report);
}

int ffb_on_get_pid_block_load_report(uint8_t *report)
{
    memcpy(report, &block_load_report, sizeof(block_load_report));
    return sizeof(struct pid_block_load_report);
}

void TC3_Handler(void)
{
    static int32_t enc_samples[2];
    uint8_t i;
    int16_t pos, speed;
    int32_t force = 0;

    PORT->Group[1].OUTCLR.reg = (1 << 30);

    TC3->COUNT16.INTFLAG.reg = TC_INTFLAG_OVF;

    /* Sample the new encoder value */
    enc_samples[1] = enc_samples[0];
    enc_samples[0] = motor_encoder_read();

    /* Compute pos, speed and acceleration coeficients */
    pos   = signed_saturate(enc_samples[0]*FFB_POSITION_COEF, 16);
    speed = signed_saturate((enc_samples[0]-enc_samples[1])*FFB_SPEED_COEF, 16);

    /* Compute individual effects */
    for (i = 0; i < FFB_MAX_EFFECTS; ++i)
        force += effect_compute(&pid_effects_pool[i], pos, speed);

    force = (force * ffb_gain) >> 8;
    force = constrain(force, -127, 127);

    /* DAC debug FFB output */
    DAC->DATA.reg = (force + 128) << 2;

    PORT->Group[1].OUTSET.reg = (1 << 30);
}
