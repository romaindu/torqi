#ifndef FFB_H
#define FFB_H

#include <stdint.h>

#define FFB_MAX_EFFECTS     8

/* Update rate should not change not to mess with FFB time units */
#define FFB_UPDATE_RATE     1000

enum effect_operation {
    EFFECT_START        = 1,
    EFFECT_START_SOLO   = 2,
    EFFECT_STOP         = 3,
};

enum device_control {
    ENABLE_ACTUATORS    = 1,
    DISABLE_ACTUATORS   = 2,
    STOP_ALL_EFFECTS    = 3,
    DEVICE_RESET        = 4,
    DEVICE_PAUSE        = 5,
    DEVICE_CONTINUE     = 6,
};

enum block_load {
    BLOCK_LOAD_SUCCESS  = 1,
    BLOCK_LOAD_FULL     = 2,
    BLOCK_LOAD_ERROR    = 3,
};

void ffb_init(void);
void ffb_reset(void);

void ffb_on_set_effect_report(uint8_t const *report);
void ffb_on_set_envelope_report(uint8_t const *report);
void ffb_on_set_condition_report(uint8_t const *report);
void ffb_on_set_periodic_report(uint8_t const *report);
void ffb_on_set_constant_force_report(uint8_t const *report);
void ffb_on_set_ramp_force_report(uint8_t const *report);
void ffb_on_create_new_effect_report(uint8_t const *report);
void ffb_on_effect_operation_report(uint8_t const *report);
void ffb_on_pid_block_free_report(uint8_t const *report);
void ffb_on_device_control_report(uint8_t const *report);
void ffb_on_device_gain_report(uint8_t const *report);
int ffb_on_get_pid_pool_report(uint8_t *report);
int ffb_on_get_pid_block_load_report(uint8_t *report);


#endif