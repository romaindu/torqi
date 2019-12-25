#ifndef REPORTS_H
#define REPORTS_H

#include <stdint.h>

struct wheel_report {
	uint8_t  report_id;
	int16_t  axis_x;
	int16_t  axis_y;
	uint8_t  axis_rx;
	uint8_t  axis_ry;
	uint8_t  axis_rz;
	uint8_t  buttons;
} __attribute__ ((__packed__));

struct set_effect_report {
	uint8_t  report_id;
	uint8_t  effect_block_index;
	uint8_t  effect_type;
	int16_t  duration;
	int16_t  trigger_repeat_interval;
	int16_t  sample_period;
	uint8_t  gain;
	uint8_t  trigger_button;
	uint8_t  axis_enable_x :1;
	uint8_t  axis_enable_y :1;
	uint8_t  :5;
	uint8_t  direction_enable :1;
	uint8_t  direction_x;
	uint8_t  direction_y;
	uint16_t start_delay;
} __attribute__ ((__packed__));

struct set_envelope_report {
	uint8_t  report_id;
	uint8_t  effect_block_index;
	int16_t  attack_level;
	int16_t  fade_level;
	int16_t  attack_time;
	int16_t  fade_time;
} __attribute__ ((__packed__));

struct set_condition_report {
	uint8_t  report_id;
	uint8_t  effect_block_index;
	uint8_t  parameter_block_offset;
	int16_t  cp_offset;
	int16_t  dead_band;
	int16_t  positive_coefficient;
	int16_t  negative_coefficient;
	int16_t  positive_saturation;
	int16_t  negative_saturation;
} __attribute__ ((__packed__));

struct set_periodic_report {
	uint8_t  report_id;
	uint8_t  effect_block_index;
	int16_t  magnitude;
	int16_t  offset;
	uint8_t  phase;
	int16_t  period;
} __attribute__ ((__packed__));

struct set_constant_force_report {
	uint8_t  report_id;
	uint8_t  effect_block_index;
	int8_t   magnitude;
} __attribute__ ((__packed__));

struct set_ramp_force_report {
	uint8_t  report_id;
	uint8_t  effect_block_index;
	int8_t   ramp_start;
	int8_t   ramp_end;
} __attribute__ ((__packed__));

struct effect_operation_report {
	uint8_t  report_id;
	uint8_t  effect_block_index;
	uint8_t  effect_operation;
	uint8_t  loop_count;
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
	uint16_t byte_count;
} __attribute__ ((__packed__));

struct pid_block_load_report {
	uint8_t  report_id;
	uint8_t  effect_block_index;
	uint8_t  block_load_status;
	uint16_t ram_pool_available;
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