/*
 * ffb.c
 *
 * @author: Romain Durand
*/

#include "ffb.h"

#include "com/serial.h"
#include "usb/hid.h"

uint16_t hid_cb_get_input(uint8_t reportID, void *buf)
{
    
}

uint16_t hid_cb_get_feature(uint8_t reportID, void *buf)
{
    switch (reportID) {
        case 3:
            puts("FFB: GET PID POOL REPORT\n");
            return 16;
    }
}
