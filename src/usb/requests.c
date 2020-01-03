/*
 * requests.c
 *
 * @author: Romain Durand
*/

#include "reports.h"

#include "tusb.h"

#include "ffb/ffb.h"

uint16_t tud_hid_get_report_cb(
    uint8_t report_id,
    hid_report_type_t report_type,
    uint8_t* buffer,
    uint16_t reqlen)
{
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    //printf("GET REPORT ID = %d, TYPE = %d\n", report_id, report_type);

    static uint8_t bi = 0;

    if (report_type == HID_REPORT_TYPE_FEATURE) {
        if (report_id == 3 && reqlen >= sizeof(struct pid_pool_report)) {
            struct pid_pool_report * sbuf = (struct pid_pool_report *)buffer;
            sbuf->report_id = 3;
            sbuf->ram_pool_size = 100;
            sbuf->simultaneous_effects = 8;
            sbuf->device_managed_pool = 1;
            sbuf->shared_parameter_block = 0;
            //printmem(buffer, sizeof(struct pid_pool_report));
            return sizeof(struct pid_pool_report);
        }

        if (report_id == 2 && reqlen >= sizeof(struct pid_block_load_report)) {
            struct pid_block_load_report * sbuf = (struct pid_block_load_report *)buffer;
            sbuf->report_id = 2;
            sbuf->effect_block_index = ++bi;
            sbuf->block_load_status = 1;
            sbuf->ram_pool_available = 90;
            //printmem(buffer, sizeof(struct pid_block_load_report));
            return sizeof(struct pid_block_load_report);
        }
    }

    return 0;
}

void tud_hid_set_report_cb(
    uint8_t report_id,
    hid_report_type_t report_type, 
    uint8_t const* buffer,
    uint16_t bufsize)
{
    (void) report_id;
    (void) report_type;

    if (!buffer || !bufsize)
        return;

    if (report_type == HID_REPORT_TYPE_FEATURE &&
            report_id == CREATE_NEW_EFFECT_REPORT_ID) {
        return ffb_on_create_new_effect_report(buffer);
    }

    switch (buffer[0]) {
        case SET_EFFECT_REPORT_ID:
            return ffb_on_set_effect_report(buffer);
        case SET_ENVELOPE_REPORT_ID:
            return ffb_on_set_envelope_report(buffer);
        case SET_CONDITION_REPORT_ID:
            return ffb_on_set_condition_report(buffer);
        case SET_PERIODIC_REPORT_ID:
            return ffb_on_set_periodic_report(buffer);
        case SET_CONSTANT_FORCE_REPORT_ID:
            return ffb_on_set_constant_force_report(buffer);
        case SET_RAMP_FORCE_REPORT_ID:
            return ffb_on_set_ramp_force_report(buffer);
        case EFFECT_OPERATION_REPORT_ID:
            return ffb_on_effect_operation_report(buffer);
        case PID_BLOCK_FREE_REPORT_ID:
            return ffb_on_pid_block_free_report(buffer);
        case PID_DEVICE_CONTROL_REPORT_ID:
            return ffb_on_device_control_report(buffer);
        case PID_DEVICE_GAIN_REPORT_ID:
            return ffb_on_device_gain_report(buffer);
        default:
            return;
    }

    //printf("SET REPORT ID = %d, TYPE = %d\n", report_id, report_type);
    //printmem(buffer, bufsize);
}
