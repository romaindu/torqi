/*
 *	Copyright 2020 Romain Durand
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
*/

#ifndef REPORTS_H
#define REPORTS_H

#include <stdint.h>

enum
{
    ITF_NUM_HID = 0,
    ITF_NUM_TOTAL
};

// Input reports
enum {
    WHEEL_REPORT_ID                 = 1,
};

// Output reports
enum {
    SET_EFFECT_REPORT_ID            = 1,
    SET_ENVELOPE_REPORT_ID          = 2,
    SET_CONDITION_REPORT_ID         = 3,
    SET_PERIODIC_REPORT_ID          = 4,
    SET_CONSTANT_FORCE_REPORT_ID    = 5,
    SET_RAMP_FORCE_REPORT_ID        = 6,
    EFFECT_OPERATION_REPORT_ID      = 10,
    PID_BLOCK_FREE_REPORT_ID        = 11,
    PID_DEVICE_CONTROL_REPORT_ID    = 12,
    PID_DEVICE_GAIN_REPORT_ID       = 13,

    VENDOR_REPORT_ID                = 20,
};

// Feature reports
enum {
    CREATE_NEW_EFFECT_REPORT_ID     = 1,
    BLOCK_LOAD_REPORT_ID            = 2,
    PID_POOL_REPORT_ID              = 3,
};

struct wheel_report {
    uint8_t  report_id;
    int16_t  axis_x;
    int16_t  axis_y;
    uint8_t  axis_rx;
    uint8_t  axis_ry;
    uint8_t  axis_rz;
    uint8_t  buttons;
} __attribute__ ((__packed__));

struct vendor_report {
    uint8_t  report_id;
    uint8_t  wheel_rotation;
};

struct ffb_constant {
    int8_t   magnitude;
} __attribute__ ((__packed__));

struct ffb_periodic {
    int8_t   magnitude;
    int8_t   offset;
    uint8_t  phase;
    uint8_t  period;
} __attribute__ ((__packed__));

struct ffb_ramp {
    int8_t   start;
    int8_t   end;
} __attribute__ ((__packed__));

struct ffb_envelope {
    int8_t   attack_level;
    int8_t   fade_level;
    uint8_t  attack_time;
    uint8_t  fade_time;
} __attribute__ ((__packed__));

struct ffb_condition {
    int8_t   cp_offset;
    int8_t   positive_coefficient;
    int8_t   negative_coefficient;
    int8_t   dead_band;
    uint8_t  positive_saturation;
    uint8_t  negative_saturation;
} __attribute__ ((__packed__));

struct ffb_effect_params {
    uint8_t  effect_type;
    uint8_t  gain;
    uint8_t  duration;
} __attribute__ ((__packed__));

struct set_effect_report {
    uint8_t  report_id;
    uint8_t  effect_block_index;
    uint8_t  payload[sizeof(struct ffb_effect_params)];
    uint8_t  ignored[3];
} __attribute__ ((__packed__));

struct set_envelope_report {
    uint8_t  report_id;
    uint8_t  effect_block_index;
    uint8_t  payload[sizeof(struct ffb_envelope)];
} __attribute__ ((__packed__));

struct set_condition_report {
    uint8_t  report_id;
    uint8_t  effect_block_index;
    uint8_t  parameter_block_offset;
    uint8_t  payload[sizeof(struct ffb_condition)];
} __attribute__ ((__packed__));

struct set_periodic_report {
    uint8_t  report_id;
    uint8_t  effect_block_index;
    uint8_t  payload[sizeof(struct ffb_periodic)];
} __attribute__ ((__packed__));

struct set_constant_force_report {
    uint8_t  report_id;
    uint8_t  effect_block_index;
    uint8_t  payload[sizeof(struct ffb_constant)];
} __attribute__ ((__packed__));

struct set_ramp_force_report {
    uint8_t  report_id;
    uint8_t  effect_block_index;
    uint8_t  payload[sizeof(struct ffb_ramp)];
} __attribute__ ((__packed__));

struct effect_operation_report {
    uint8_t  report_id;
    uint8_t  effect_block_index;
    uint8_t  effect_operation;
} __attribute__ ((__packed__));

struct pid_block_free_report {
    uint8_t  report_id;
    uint8_t  effect_block_index;
} __attribute__ ((__packed__));

struct pid_device_control_report {
    uint8_t  report_id;
    uint8_t  pid_device_control;
} __attribute__ ((__packed__));

struct device_gain_report {
    uint8_t  report_id;
    uint8_t  gain;
} __attribute__ ((__packed__));

struct create_new_effect_report {
    uint8_t  report_id;
    uint8_t  effect_type;
} __attribute__ ((__packed__));

struct pid_block_load_report {
    uint8_t  report_id;
    uint8_t  effect_block_index;
    uint8_t  block_load_status;
    uint8_t  ram_pool_available;
} __attribute__ ((__packed__));

struct pid_pool_report {
    uint8_t  report_id;
    uint8_t  ram_pool_size;
    uint8_t  simultaneous_effects;
    uint8_t  device_managed_pool :1;
    uint8_t  shared_parameter_block :1;
    uint8_t  :6;
} __attribute__ ((__packed__));

#endif
