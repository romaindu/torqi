#ifndef HID_H
#define HID_H

#include <stdint.h>
#include "descriptors.h"
#include "usb.h"

/* Functions called externally */


/* Functions called by usb.c */
void hid_configure(uint16_t confNum);
void hid_on_setup_request(SetupPacket_t setupPacket);
void hid_on_in_xfer(uint8_t ep);
void hid_on_out_xfer(uint8_t ep, uint8_t bc);

/* Callbacks */
uint16_t hid_cb_get_input(uint8_t reportID, void *buf);
void hid_cb_set_output(uint8_t reportID, void *buf);
uint16_t hid_cb_get_feature(uint8_t reportID, void *buf);
void hid_cb_set_feature(uint8_t reportID, void *buf);

#endif