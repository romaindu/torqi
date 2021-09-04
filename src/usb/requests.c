/*
 * requests.c
 *
 * @author: Romain Durand
*/

#include "reports.h"

#include "tusb.h"

#include "ffb/ffb.h"
#include "whl/wheel.h"

void _on_set_vendor_report(uint8_t const *report)
{
    struct vendor_report *r = (struct vendor_report *)report;

    /* Setting various globals */
    steering_range = 5 * r->wheel_rotation;
}

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

    if (report_type == HID_REPORT_TYPE_FEATURE) {
        if (report_id == PID_POOL_REPORT_ID) {
            return ffb_on_get_pid_pool_report(buffer);
        }

        if (report_id == BLOCK_LOAD_REPORT_ID) {
            return ffb_on_get_pid_block_load_report(buffer);
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

    if (report_type == HID_REPORT_TYPE_FEATURE) {
        switch (report_id) {
            case CREATE_NEW_EFFECT_REPORT_ID:
                return ffb_on_create_new_effect_report(buffer);
        }
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

        case VENDOR_REPORT_ID:
            return _on_set_vendor_report(buffer);
        default:
            return;
    }
}
